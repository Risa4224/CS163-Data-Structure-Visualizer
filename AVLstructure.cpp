#include "AVLstructure.h"
#include <iostream>
#include <algorithm>

using namespace std;

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree()
{
    destroyTree(root);
}

void AVLTree::destroyTree(Node *node)
{
    if (node)
    {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

int AVLTree::height(Node *node)
{
    return node ? node->height : 0;
}

int AVLTree::getBalanceFactor(Node *node)
{
    return node ? height(node->left) - height(node->right) : 0;
}

Node *AVLTree::rotateRight(Node *y)
{
    Node *x = y->left;
    takeSnapshot(y->data, "Rotate Right: x = y->left", 0, "ROT_R");

    Node *T2 = x->right;
    takeSnapshot(y->data, "Rotate Right: T2 = x->right", 1, "ROT_R");

    x->right = y;
    y->left = T2;
    takeSnapshot(x->data, "Re-linking nodes...", 2, "ROT_R");

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    takeSnapshot(x->data, "Update heights", 3, "ROT_R");

    return x;
}

Node *AVLTree::rotateLeft(Node *x)
{
    Node *y = x->right;
    takeSnapshot(x->data, "Rotate Left: y = x->right", 0, "ROT_L");

    Node *T2 = y->left;
    takeSnapshot(x->data, "Rotate Left: T2 = y->left", 1, "ROT_L");

    y->left = x;
    x->right = T2;
    takeSnapshot(y->data, "Re-linking nodes...", 2, "ROT_L");

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    takeSnapshot(y->data, "Update heights", 3, "ROT_L");

    return y;
}

Node *AVLTree::balance(Node *&node)
{
    int balanceFactor = getBalanceFactor(node);

    // LL Case
    if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0)
    {
        takeSnapshot(node->data, "LL Case: Right Rotation needed", 0, "BALANCE");
        node = rotateRight(node);
        return node;
    }

    // LR Case
    if (balanceFactor > 1 && getBalanceFactor(node->left) < 0)
    {
        takeSnapshot(node->left->data, "LR Case: Left Rotation on child first", 1, "BALANCE");
        node->left = rotateLeft(node->left);
        takeSnapshot(node->data, "LR Case: Now Right Rotation on node", 1, "BALANCE");
        node = rotateRight(node);
        return node;
    }

    // RR Case
    if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0)
    {
        takeSnapshot(node->data, "RR Case: Left Rotation needed", 2, "BALANCE");
        node = rotateLeft(node);
        return node;
    }

    // RL Case
    if (balanceFactor < -1 && getBalanceFactor(node->right) > 0)
    {
        takeSnapshot(node->right->data, "RL Case: Right Rotation on child first", 3, "BALANCE");
        node->right = rotateRight(node->right);
        takeSnapshot(node->data, "RL Case: Now Left Rotation on node", 3, "BALANCE");
        node = rotateLeft(node);
        return node;
    }

    return node;
}

Node *AVLTree::insert(Node *&node, int data)
{
    if (!node)
    {
        node = new Node(data);
        node->left = node->right = nullptr;
        node->height = 1;
        return node;
    }

    if (data < node->data)
    {
        takeSnapshot(node->data, "data < node->data: Go Left", 0, "INS_L");
        node->left = insert(node->left, data);
    }
    else if (data > node->data)
    {
        takeSnapshot(node->data, "data > node->data: Go Right", 0, "INS_R");
        node->right = insert(node->right, data);
    }
    else
    {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    takeSnapshot(node->data, "Backtracking and checking balance...", 1, "BALANCE");
    return balance(node);
}

void AVLTree::insert(int data)
{
    animationStates.clear();
    takeSnapshot(-1, "Preparing to insert value: " + to_string(data), 0, "API");

    bool wasEmpty = (root == nullptr);
    root = insert(root, data);

    if (wasEmpty)
    {
        takeSnapshot(data, "Tree was empty. Node " + to_string(data) + " becomes root.", 1, "API");
    }

    takeSnapshot(-1, "Insertion process finished.", 2, "API");
}

Node *AVLTree::remove(Node *&node, int data)
{
    if (!node)
        return nullptr;

    if (data < node->data)
    {
        takeSnapshot(node->data, "data < node->data: Search Left", 0, "DEL");
        node->left = remove(node->left, data);
    }
    else if (data > node->data)
    {
        takeSnapshot(node->data, "data > node->data: Search Right", 0, "DEL");
        node->right = remove(node->right, data);
    }
    else
    {
        // Tìm thấy node cần xóa
        if (!node->left || !node->right)
        {
            takeSnapshot(node->data, "Deleting node with < 2 children", 1, "DEL");
            Node *temp = node->left ? node->left : node->right;
            if (!temp)
            {
                temp = node;
                node = nullptr;
            }
            else
            {
                Node *toDelete = node;
                node = temp;
                temp = toDelete;
            }
            delete temp;
        }
        else
        {
            takeSnapshot(node->data, "Node has 2 children, find successor", 2, "DEL");
            Node *temp = minValueNode(node->right);
            node->data = temp->data;
            takeSnapshot(node->data, "Replacement found, delete successor", 3, "DEL");
            node->right = remove(node->right, temp->data);
        }
    }

    if (!node)
        return nullptr;
    node->height = 1 + max(height(node->left), height(node->right));
    return balance(node);
}

void AVLTree::remove(int data)
{
    animationStates.clear();
    takeSnapshot(-1, "Starting deletion process for: " + to_string(data), 0, "API");

    root = remove(root, data);

    takeSnapshot(-1, "Deletion process completed.", 1, "API");
}

bool AVLTree::search(int data)
{
    animationStates.clear();
    Node *current = root;
    while (current)
    {
        takeSnapshot(current->data, "Checking node " + to_string(current->data), 0, "SEARCH");

        if (data == current->data)
        {
            takeSnapshot(current->data, "Value found: " + to_string(data), 1, "SEARCH");
            return true;
        }

        if (data < current->data)
        {
            takeSnapshot(current->data, to_string(data) + " < " + to_string(current->data) + " -> Go Left", 2, "SEARCH");
            current = current->left;
        }
        else
        {
            takeSnapshot(current->data, to_string(data) + " > " + to_string(current->data) + " -> Go Right", 3, "SEARCH");
            current = current->right;
        }
    }
    takeSnapshot(-1, "Value " + to_string(data) + " not in tree.", 4, "SEARCH");
    return false;
}
void AVLTree::takeSnapshot(int highlightData, std::string action, int lineID, std::string codeType)
{
    TreeState newState;
    newState.highlightData = highlightData;
    newState.action = action;
    newState.lineID = lineID;
    newState.currentCodeType = codeType;

    float startX = 700.0f;
    float startY = 150.0f;
    float initialOffset = 180.0f;

    copyTreeToSnapshot(root, newState.captures, startX, startY, initialOffset);
    animationStates.push_back(newState);
}

void AVLTree::inorder(Node *node)

{

    if (node)

    {

        inorder(node->left);

        cout << node->data << " ";

        inorder(node->right);
    }
}

void AVLTree::inorderTraversal()

{

    inorder(root);

    cout << endl;
}

void AVLTree::preorder(Node *node)

{

    if (node)

    {

        cout << node->data << " ";

        preorder(node->left);

        preorder(node->right);
    }
}

void AVLTree::preorderTraversal()

{

    preorder(root);

    cout << endl;
}

void AVLTree::postorder(Node *node)

{

    if (node)

    {

        postorder(node->left);

        postorder(node->right);

        cout << node->data << " ";
    }
}

void AVLTree::postorderTraversal()

{

    postorder(root);

    cout << endl;
}

//

int AVLTree::getHeight()

{

    return height(root);
}

//

bool AVLTree::isEmpty()

{

    return root == nullptr;
}

void AVLTree::clear()
{

    destroyTree(root);

    root = nullptr;

    animationStates.clear();
}

Node *AVLTree::minValueNode(Node *node)
{
    Node *current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

int AVLTree::copyTreeToSnapshot(Node *node, std::vector<NodeState> &snapshot, float x, float y, float offset)
{
    if (node == nullptr)
        return -1;

    NodeState newState;
    newState.data = node->data;
    newState.height = node->height;
    newState.x = x;
    newState.y = y;

    snapshot.push_back(newState);
    int currentIndex = snapshot.size() - 1;

    float nextY = y + 80.0f;
    float nextOffset = offset / 2.0f;

    int leftIdx = copyTreeToSnapshot(node->left, snapshot, x - offset, nextY, nextOffset);
    int rightIdx = copyTreeToSnapshot(node->right, snapshot, x + offset, nextY, nextOffset);

    snapshot[currentIndex].left = leftIdx;
    snapshot[currentIndex].right = rightIdx;

    return currentIndex;
}