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

Node *AVLTree::balance(Node *node) // State-tracking missing
{
    int balance = getBalanceFactor(node);

    if (balance > 1 && getBalanceFactor(node->left) >= 0)
        return rotateRight(node);

    if (balance > 1 && getBalanceFactor(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && getBalanceFactor(node->right) <= 0)
        return rotateLeft(node);

    if (balance < -1 && getBalanceFactor(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// process

Node *AVLTree::insert(Node *node, int data) // State-tracking missing
{
    if (!node)
        return new Node(data);

    if (data < node->data)
        node->left = insert(node->left, data);
    else if (data > node->data)
        node->right = insert(node->right, data);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    return balance(node);
}

void AVLTree::insert(int data)
{
    root = insert(root, data);
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

Node *AVLTree::remove(Node *node, int data) // State-tracking missing
{
    if (!node)
        return node;

    if (data < node->data)
        node->left = remove(node->left, data);
    else if (data > node->data)
        node->right = remove(node->right, data);
    else
    {
        if (!node->left || !node->right)
        {
            Node *temp = node->left ? node->left : node->right;
            if (!temp)
            {
                temp = node;
                node = nullptr;
            }
            else
                *node = *temp;
            delete temp;
        }
        else
        {
            Node *temp = minValueNode(node->right);
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
    root = remove(root, data);
}

//

bool AVLTree::search(int data) // State-tracking missing
{
    Node *current = root;
    while (current)
    {
        if (data == current->data)
            return true;
        else if (data < current->data)
            current = current->left;
        else
            current = current->right;
    }
    return false;
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