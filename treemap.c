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
    if (node == NULL) return;

    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent != NULL)
        {
            if (node->parent->left == node)    node->parent->left = NULL;
            if (node->parent->right == node)    node->parent->right = NULL;
        }
        free(node);
        return;
    }
    else if (node->left == NULL || node->right == NULL)
    {
        if (node->parent != NULL)
        {
            if (node->parent->left == node && node->right != NULL)    
            {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
            if (node->parent->right == node  && node->right != NULL)    
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
            if (node->parent->left == node && node->left != NULL)    
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }
            if (node->parent->right == node  && node->left != NULL)    
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
        }
        free(node);
        return;
    }
    else
    {
        TreeNode* minimo = minimum(node->right);
        node->pair->key = minimo->pair->key;
        node->pair->value = minimo->pair->value;
        removeNode(tree, minimo);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


/* NOTA: HACER SEARCH ANTES QUE REMOVE */

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree->root == NULL || tree == NULL)
         return NULL;
    else
    {
        tree->current = tree->root;
        while (tree->current != NULL)
        {
            if (tree->lower_than(key, tree->current->pair->key))
                tree->current = tree->current->left;
            else
            {
                if (is_equal(tree, tree->current->pair->key, key))
                {
                    return tree->current->pair;
                }
                tree->current = tree->current->right;
            }
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    tree->current = tree->root;
    while (tree->current->left != NULL)
    {
        tree->current = tree->current->left;
    }
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree->current == NULL) return NULL;
    void* key = tree->current->pair->key;
    if (tree->current->right != NULL)
    {
        tree->current = minimum(tree->current->right);
        return tree->current->pair;
    }
    while (tree->current->parent != NULL && tree->current->parent->pair->key <= key)
    {
        tree->current = tree->current->parent;
    }
    if (tree->current->pair->key < key)
        return NULL;
    else
        return tree->current->pair;
}
