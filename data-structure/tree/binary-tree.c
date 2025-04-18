#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

Node* node(int data) {
    Node *node = malloc(sizeof(Node));

    if(!node)
        return NULL;

    node->data = data;
    node->left = node->right = NULL;

    return node;
}

void preOrder(Node* node) {
    if(!node)
        return;

    printf("%d ", node->data);
    preOrder(node->left);
    preOrder(node->right);
}

void postOrder(Node *node) {
    if(!node)
        return;

    postOrder(node->left);
    postOrder(node->right);
    printf("%d ", node->data);
}

void inOrder(Node *node) {
    if(!node)
        return;

    inOrder(node->left);
    printf("%d ", node->data);
    inOrder(node->right);
}

bool isFullBinaryTree(Node *root) {
    if(!root)
        return true;

    if(root->left == NULL && root->right == NULL)
        return true;

    if(root->left && root->right) 
        return (isFullBinaryTree(root->left) && isFullBinaryTree(root->right));
    
    
    return false;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int checkHeight(Node *node, bool *isBalanced) {
    if(!node)
        return 0;

    int leftHeight = checkHeight(node->left, isBalanced);
    int rightHeight = checkHeight(node->right, isBalanced);

    if(abs(leftHeight - rightHeight) > 1)
        *isBalanced = false;

    return max(leftHeight, rightHeight) + 1;
}

bool isBalanced(Node *root) {
    bool balanced = true;
    checkHeight(root, &balanced);

    return balanced;
}

int main() {
    Node *root = node(1);
    root->left = node(12);
    root->left->left = node(5);
    root->left->right = node(6);
    root->right = node(9);

    printf("Pre-order: ");
    preOrder(root);
    printf("\nPost-order: ");
    postOrder(root);
    printf("\nIn-order: ");
    inOrder(root);
    printf("\nis a full binary tree? %d", isFullBinaryTree(root));
    printf("\nis balanced? %d\n", isBalanced(root));
}