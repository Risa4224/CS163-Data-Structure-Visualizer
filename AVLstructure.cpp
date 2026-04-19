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

Node *AVLTree::balance(Node *node)
{
    int balanceFactor = getBalanceFactor(node);

    takeSnapshot(node->data, "Checking Balance Factor for " + to_string(node->data), 0);

    // LL Case
    if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0)
    {
        takeSnapshot(node->data, " LL Unbalanced.", 0);
        return rotateRight(node);
    }

    // LR Case
    if (balanceFactor > 1 && getBalanceFactor(node->left) < 0)
    {
        takeSnapshot(node->left->data, " LR Unbalanced.", 0);
        node->left = rotateLeft(node->left);
        takeSnapshot(node->data, "LR Case:", 0);
        return rotateRight(node);
    }

    // RR Case
    if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0)
    {
        takeSnapshot(node->data, " RR Unbalanced.", 0);
        return rotateLeft(node);
    }

    // RL Case
    if (balanceFactor < -1 && getBalanceFactor(node->right) > 0)
    {
        takeSnapshot(node->right->data, " RL Unbalanced.", 0);
        node->right = rotateRight(node->right);
        takeSnapshot(node->data, "RL Case:", 0);
        return rotateLeft(node);
    }

    return node;
}
// process

Node *AVLTree::insert(Node *node, int data)
{
    if (!node)
    {
        Node *newNode = new Node(data);
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;

        takeSnapshot(data, "Founded! Inserting: " + to_string(data), 0);
        return newNode;
    }

    takeSnapshot(node->data, "Comparing " + to_string(data) + " with " + to_string(node->data), 0);

    if (data < node->data)
        node->left = insert(node->left, data);
    else if (data > node->data)
        node->right = insert(node->right, data);
    else
    {
        takeSnapshot(node->data, "Value " + to_string(data) + " already exists!", 0);
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    return balance(node);
}

void AVLTree::insert(int data)
{
    animationStates.clear();
    takeSnapshot(-1, "Bat dau chen: " + to_string(data), 0);
    root = insert(root, data);
    takeSnapshot(-1, "Hoan tat thao tac chen " + to_string(data), 0);
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

Node *AVLTree::remove(Node *node, int data)
{
    if (!node)
    {
        takeSnapshot(-1, "Cannot find " + to_string(data) + " to delete.", 0);
        return node;
    }

    takeSnapshot(node->data, "Searching for deletion: currently examining " + to_string(node->data), 0);

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
                *node = *temp;
            }
            delete temp;
            takeSnapshot(-1, "Deleted node (case with 0 or 1 child).", 0);
        }
        else
        {
            takeSnapshot(node->data, "Node has 2 children: Finding minimum node in right subtree...", 0);
            Node *temp = minValueNode(node->right);

            takeSnapshot(temp->data, "Node to replace with: " + to_string(temp->data), 0);
            node->data = temp->data;
            node->right = remove(node->right, temp->data);
        }
    }

    if (!node)
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    return balance(node);
}

void AVLTree::remove(int data)
{
    animationStates.clear();
    takeSnapshot(-1, "Bat dau xoa: " + to_string(data), 0);
    root = remove(root, data);
    takeSnapshot(-1, "Hoan tat thao tac xoa " + to_string(data), 0);
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