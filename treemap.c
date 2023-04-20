#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* new = (TreeMap*) malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* newNode = createTreeNode(key, value);
    if (tree->root == NULL)
        tree->root = newNode;
    else
    {
        tree->current = tree->root;
        TreeNode* auxNode = createTreeNode(tree->current->pair->key, tree->current->pair->value);
        while (tree->current != NULL)
        {
            auxNode = tree->current;
            if (tree->lower_than(newNode->pair->key, tree->current->pair->key))
                tree->current = tree->current->left;
            else
            {
                if (is_equal(tree, tree->current->pair->key, newNode->pair->key))
                {
                    tree->current = NULL;
                    return;
                }
                tree->current = tree->current->right;
            }
        }
        tree->current = auxNode;
        if (tree->lower_than(newNode->pair->key, auxNode->pair->key))
            tree->current->left = newNode;
        else
            tree->current->right = newNode;
    }
    newNode->parent = tree->current;
    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL)
        x = x->left;
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree->root == NULL)
        return;
    tree->current = tree->root;
    while (!is_equal(tree, node->pair->key, tree->current->pair->key) && tree->current != NULL)
    {
        if (tree->lower_than(node->pair->key, tree->current->pair->key))
            tree->current = tree->current->left;
        else
            tree->current = tree->current->right;
    }
    if (tree->current != NULL)
    {
        if (tree->current->left == NULL && tree->current->right == NULL)
        {
            if (tree->lower_than(node->pair->key, tree->current->parent->pair->key))
                tree->current->parent->left = NULL;
            else
                tree->current->parent->right = NULL;
            tree->current = NULL;
            free(tree->current);
            return;
        }
        if (tree->current->left != NULL && tree->current->right != NULL)
        {
            TreeNode*minNode = minimum(tree->current->right);
            tree->current->pair->value = minNode->pair->value;
            tree->current->pair->key = minNode->pair->key;
            minNode = NULL;
            tree->current = NULL;
            return;
        }
        else
        {
            TreeNode* child = (tree->current->left != NULL) ? tree->current->left : tree->current->right;
            if (tree->current == tree->root)
                tree->root = child;
            else if (tree->current == tree->current->parent->left)
                tree->current->parent->left = child;
            else
                tree->current->parent->right = child;
            child->parent = tree->current->parent;
        }
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
