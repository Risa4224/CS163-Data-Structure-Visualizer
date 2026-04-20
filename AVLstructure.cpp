#include "AVLstructure.h"
#include <iostream>
#include <algorithm>

using namespace std;

// logic

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
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node *AVLTree::rotateLeft(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

Node *AVLTree::balance(Node *&node)
{
    int balanceFactor = getBalanceFactor(node);

    // LL Case
    if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0)
    {
        takeSnapshot(node->data, "LL unbalanced. Rotating right.", 0);
        node = rotateRight(node);
        takeSnapshot(node->data, "Done rotating right. Node " + to_string(node->data) + " is now the root.", 0);
        return node;
    }

    // LR Case
    if (balanceFactor > 1 && getBalanceFactor(node->left) < 0)
    {
        takeSnapshot(node->left->data, "LR unbalanced. Rotating left.", 0);
        node->left = rotateLeft(node->left);
        takeSnapshot(node->data, "Continuing to rotate right.", 0);
        node = rotateRight(node);
        takeSnapshot(node->data, "Done rotating right.", 0);
        return node;
    }

    // RR Case
    if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0)
    {
        takeSnapshot(node->data, "RR unbalanced. Rotating left.", 0);
        node = rotateLeft(node);
        takeSnapshot(node->data, "Done rotating left. Node " + to_string(node->data) + " is now the root.", 0);
        return node;
    }

    // RL Case
    if (balanceFactor < -1 && getBalanceFactor(node->right) > 0)
    {
        takeSnapshot(node->right->data, "RL unbalanced. Rotating right.", 0);
        node->right = rotateRight(node->right);
        takeSnapshot(node->data, "Continuing to rotate left.", 0);
        node = rotateLeft(node);
        takeSnapshot(node->data, "Done rotating left.", 0);
        return node;
    }

    return node;
}
// process

Node *AVLTree::insert(Node *&node, int data)
{
    if (!node)
    {
        node = new Node(data);
        node->left = nullptr;
        node->right = nullptr;
        node->height = 1;
        return node;
    }

    takeSnapshot(node->data, "Comparing " + to_string(data) + " with " + to_string(node->data), 0);

    if (data < node->data)
    {
        bool isNull = (node->left == nullptr);
        node->left = insert(node->left, data);
        if (isNull)
            takeSnapshot(data, "Inserted " + to_string(data) + " to the left", 0);
    }
    else if (data > node->data)
    {
        bool isNull = (node->right == nullptr);
        node->right = insert(node->right, data);
        if (isNull)
            takeSnapshot(data, "Inserted " + to_string(data) + " to the right", 0);
    }
    else
    {
        takeSnapshot(node->data, "Value " + to_string(data) + " already exists!", 0);
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int bf = getBalanceFactor(node);
    if (bf >= -1 && bf <= 1)
    {
        takeSnapshot(node->data, "Rotating back: Node " + to_string(node->data) + " is balanced", 0);
    }

    return balance(node);
}

void AVLTree::insert(int data)
{
    animationStates.clear();
    takeSnapshot(-1, "Inserting: " + to_string(data), 0);

    bool wasEmpty = (root == nullptr);
    root = insert(root, data);
    if (wasEmpty)
    {
        takeSnapshot(data, "Inserted root node " + to_string(data), 0);
    }

    takeSnapshot(-1, "Insert Completed " + to_string(data), 0);
}

//

Node *AVLTree::minValueNode(Node *node)
{
    Node *current = node;
    while (current->left)
        current = current->left;
    return current;
}

//

Node *AVLTree::remove(Node *&node, int data)
{
    if (!node)
    {
        takeSnapshot(-1, "Value " + to_string(data) + " not found to delete.", 0);
        return nullptr;
    }

    takeSnapshot(node->data, "Searching for " + to_string(data) + ": checking node " + to_string(node->data), 0);

    if (data < node->data)
        node->left = remove(node->left, data);
    else if (data > node->data)
        node->right = remove(node->right, data);
    else
    {
        takeSnapshot(node->data, "Found node to delete: " + to_string(data), 0);

        if (!node->left || !node->right)
        {
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
            takeSnapshot(-1, "Deleted node " + to_string(data), 0);
        }
        else
        {
            takeSnapshot(node->data, "Node has 2 children: finding successor in right subtree...", 0);
            Node *temp = minValueNode(node->right);

            takeSnapshot(temp->data, "Replacement found: Node " + to_string(temp->data), 0);
            node->data = temp->data;
            node->right = remove(node->right, temp->data);
        }
    }

    if (!node)
        return nullptr;

    node->height = 1 + max(height(node->left), height(node->right));

    takeSnapshot(node->data, "Backtracking: checking balance at node " + to_string(node->data), 0);

    return balance(node);
}

void AVLTree::remove(int data)
{
    animationStates.clear();
    takeSnapshot(-1, "Starting deletion of: " + to_string(data), 0);
    root = remove(root, data);
    takeSnapshot(-1, "Finished deletion of " + to_string(data), 0);
}
//

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

bool AVLTree::search(int data)
{
    animationStates.clear();
    Node *current = root;
    while (current)
    {
        takeSnapshot(current->data, "Checking node " + to_string(current->data), 0);

        if (data == current->data)
        {
            takeSnapshot(current->data, "Success! Found " + to_string(data), 0);
            return true;
        }
        else if (data < current->data)
        {
            takeSnapshot(current->data, to_string(data) + " is less than, moving to the left", 0);
            current = current->left;
        }
        else
        {
            takeSnapshot(current->data, to_string(data) + " is greater than, moving to the right", 0);
            current = current->right;
        }
    }
    takeSnapshot(-1, "Failed to find " + to_string(data) + " in the tree.", 0);
    return false;
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

void AVLTree::takeSnapshot(int highlightData, std::string action, int codeLine)
{
    TreeState newState;
    newState.highlightData = highlightData;
    newState.action = action;
    newState.codeLine = codeLine;

    float startX = 750.0f;
    float startY = 80.0f;
    float initialOffset = 250.0f;

    copyTreeToSnapshot(root, newState.captures, startX, startY, initialOffset);

    animationStates.push_back(newState);
}

void AVLTree::clear()
{

    destroyTree(root);

    root = nullptr;

    animationStates.clear();
}