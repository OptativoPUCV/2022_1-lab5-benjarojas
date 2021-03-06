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

    TreeMap* new = (TreeMap *) malloc(sizeof(TreeMap));
    new->lower_than = lower_than;
    new->root = NULL;
    new->current = NULL;

    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    
    TreeNode *auxNode= tree->root;
    tree->current = tree->root;

    int searchComp;

    while(1)
    {
        if(is_equal(tree, key, auxNode->pair->key)) return;

        searchComp = tree->lower_than(key,auxNode->pair->key);
     
        if(searchComp)
        {
            if(auxNode->left == NULL)
            {
                TreeNode *insertAux = createTreeNode(key, value);
                auxNode->left = insertAux;
                insertAux->parent = auxNode;
                tree->current = insertAux;
                return;
            }
            auxNode = auxNode->left;
            tree->current = auxNode;
        } else {
            if(auxNode->right == NULL)
            {
                TreeNode *insertAux = createTreeNode(key, value);
                auxNode->right = insertAux;
                insertAux->parent = auxNode;
                tree->current = insertAux;
                return;
            }
            auxNode = auxNode->right;
            tree->current = auxNode;
        }
    }
}

TreeNode * minimum(TreeNode * x){

    TreeNode *auxNode = x;

    while(auxNode->left != NULL)
        auxNode = auxNode->left;
    
    return auxNode;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    
    // nodo sin hijos
    if((node->left == NULL) && (node->right == NULL))
    {
        if(node->parent->right == node)
            node->parent->right = NULL;

        if(node->parent->left == node)
            node->parent->left = NULL;

        tree->current = node->parent;
    }
        
    if((node->left != NULL) && (node->right == NULL))
    {
        node->pair = node->left->pair;
        node->left->parent = node->parent;
        node->parent->left = node->left;
        tree->current = node->left;
    }

    if((node->right != NULL) && (node->left == NULL))
    {
        node->pair = node->right->pair;
        node->right->parent = node->parent;
        node->parent->right = node->right;
        tree->current = node->right;
    }

    if((node->right != NULL) && (node->left != NULL))
    {
        TreeNode *deleteAux;
        deleteAux = node->right;

        deleteAux = minimum(deleteAux);

        node->pair->key = deleteAux->pair->key;
        node->pair->value = deleteAux->pair->value;

        tree->current = node;

        removeNode(tree, deleteAux);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {

    TreeNode *auxNode= tree->root;
    tree->current = tree->root;

    int searchComp;

    while(1)
    {
        if(is_equal(tree, key, auxNode->pair->key))
        {
            tree->current = auxNode;
            return auxNode->pair;
        }
     
        searchComp = tree->lower_than(key,auxNode->pair->key);
     
        if(searchComp)
        {
            auxNode = auxNode->left;
            tree->current = auxNode;
        } else {
            auxNode = auxNode->right;
            tree->current = auxNode;
        }

        if(auxNode == NULL) return NULL;
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {

    TreeNode *auxNode = tree->root;
    tree->current = tree->root;

    while(auxNode->left != NULL)
        auxNode = auxNode->left;
        
    tree->current = auxNode;

    return auxNode->pair;
}

Pair * nextTreeMap(TreeMap * tree) {

    if(tree == NULL || tree->current == NULL)
        return NULL;

    TreeNode *nextAux;
    if(tree->current->right)
        nextAux = minimum(tree->current->right);
    else {
        nextAux = tree->current->parent;
        while(nextAux != NULL && tree->lower_than(nextAux->pair->key, tree->current->pair->key))
            nextAux = nextAux->parent;
    }
    tree->current = nextAux;
    if(nextAux)
        return nextAux->pair;
    return NULL;
}
