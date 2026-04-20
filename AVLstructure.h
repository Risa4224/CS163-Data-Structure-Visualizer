#ifndef AVLSTRUCTURE_H
#define AVLSTRUCTURE_H
#include <vector>
#include <string>

struct Node
{
    int data;
    Node *left;
    Node *right;
    int height;
    Node(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
};

struct NodeState
{
    int data;
    int x, y; //-> UI
    int height;
    int left;
    int right;
};

struct TreeState
{
    std::vector<NodeState> captures;
    std::string action;
    int codeLine;
    int highlightData;

    std::string description;
};
class AVLTree
{
public:
    Node *root;
    AVLTree();
    ~AVLTree();
    void insert(int data);
    void remove(int data);
    bool search(int data);
    void inorderTraversal();
    void preorderTraversal();
    void postorderTraversal();
    int getHeight();
    bool isEmpty();
    void clear();
    // State-tracking
    std::vector<TreeState> animationStates;

private:
    Node *insert(Node *&node, int data);
    Node *remove(Node *&node, int data);
    Node *balance(Node *&node);
    int getBalanceFactor(Node *node);
    int height(Node *node);
    Node *rotateLeft(Node *node);
    Node *rotateRight(Node *node);
    Node *minValueNode(Node *node);
    void inorder(Node *node);
    void preorder(Node *node);
    void postorder(Node *node);
    void destroyTree(Node *node);

    int copyTreeToSnapshot(Node *node, std::vector<NodeState> &snapshot, float x, float y, float offset);
    void takeSnapshot(int highlightData, std::string action, int codeLine);
};

#endif