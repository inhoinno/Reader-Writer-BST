#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "include/lab2_sync_types.h"
#include "include/rwlock.h"

#define ERR_NO_SUCH_KEY -1


int Reader(lab2_tree *tree, int val){
    //lock
    rwlock_acquire_readlock(tree->rw);
    /*Critical Section*/
    int ret ;
    lab2_node *node =NULL;
    if(tree->root != NULL){
        node = tree->root;
        ret= searcher(node , val);
    }
    /*Critical Section END*/
    rwlock_release_readlock(tree->rw);
    return ret;
}

int searcher(lab2_node *node , int value){
    //base on value, return key;
    lab2_node * iter = node;
    while (iter != NULL){
        if(iter->value == value)
            return iter->key;
        else if (iter->value > value){
            iter = iter->left;
        }else
        {
            iter = iter->right;
        }
    }
    if(iter == NULL)
        return ERR_NO_SUCH_KEY ;
}

lab2_node* _searcher_node(lab2_node *node , int value){
    //base on value, return key;
    lab2_node * iter = node;
    while (iter != NULL){
        if(iter->value == value)
            return iter;
        else if (iter->value > value){
            iter = iter->left;
        }else
        {
            iter = iter->right;
        }
    }
    if(iter == NULL)
        return NULL; //ERR_NO_SUCH_KEY ;
}
lab2_node* _searcher_parent_node(lab2_node *node , int value){
    //base on value, return key;
    lab2_node * p_iter = node->parent;
    lab2_node * iter = node;
    
    if (node->parent == NULL){        // if node is root
        p_iter = node;
        iter = node->value < value ? node->right : node->left; 
    }


    while (iter != NULL){
        if(iter->value == value)
            return p_iter;
        else if (iter->value > value){
            p_iter = iter;
            iter = iter->left;
        }else
        {
            p_iter = iter;
            iter = iter->right;
        }
    }
    return NULL; //ERR_NO_SUCH_KEY ;
}


int Writer_insert_cg(lab2_tree *tree, lab2_node * node){
    //lock
    int val = node->value;
    rwlock_acquire_writelock(tree->rw);
    /*Critical Section*/
    int ret = 1;
    int cond = 1;
    lab2_node *p_iter = tree->root;
    lab2_node *iter = tree->root;
    //CASE Root is Empty
    if (tree->root == NULL){        // if root is empty
        //1. Create Node
        //2. tree->root = 
        tree->root = node;
        cond=0;
    }
    //CASE normal
    if(cond != 0){
        while(iter != NULL){
            if(iter->value > val){
                p_iter = iter;
                iter = iter->left;
            }
            else{ // same or over
                p_iter = iter;
                iter = iter->right;
            }
        }     

        if(p_iter->value > val ){
            p_iter->left = node;
        }
        else{
            p_iter->right = node;
        }     
    }
    /*Critical Section END*/
    rwlock_release_writelock(tree->rw);
    return ret;
}


int Writer_insert_fg(lab2_tree *tree, lab2_node * node){
    //lock
    int val = node->value;
    rwlock_acquire_writelock(tree->rw);
    
    /*Critical Section*/
    int ret = 1;
    int cond = 0;
    lab2_node *p_iter = tree->root;
    lab2_node *iter = tree->root;
    //CASE Root is Empty
    if (tree->root == NULL){        // if root is empty
        //1. Create Node
        iter = lab2_node_create(val);
        //2. tree->root = 
        tree->root = iter;
        cond=0;
    }
    //CASE normal
    if(cond != 0){
        while(iter != NULL ){
            if(iter->value > val){
                p_iter = iter;
                iter = iter->left;
            }
            else{ // same or over
                p_iter = iter;
                iter = iter->right;
            }
        }     

        if(p_iter->value > val ){
            p_iter->left = lab2_node_create(val);
        }
        else{
            p_iter->right = lab2_node_create(val);
        }     
    }
    /*Critical Section END*/
    rwlock_release_writelock(tree->rw);
    return ret;
}

int Writer_delete_cg(lab2_tree *tree, int val){
    //lock
    rwlock_acquire_writelock(tree->rw);
    /*Critical Section*/
    int ret ;
    int cond = 1;
    lab2_node *p_iter = tree->root;
    lab2_node *iter = tree->root;
    //0. if root is empty
    if (tree->root == NULL){        
        //case fail
        cond = 0;
        ret = ERR_NO_SUCH_KEY;
    }
    //1. Search p_iter , iter
    if(cond != 0){
        while(iter != NULL){
            if(iter->value == val){
                ret = 1;
                cond = 1;
                break; //find Key
            }
            else if(iter->value < val){ // same or over
                p_iter = iter;
                iter = iter->right;
            }else{
                p_iter = iter;
                iter = iter->left;
            }
        } 
        if(iter == NULL)   
            cond = 0;

        //2. iter->child 0 , 1 , 2
        if(cond != 0){
            if(iter->left != NULL && iter->right != NULL){
                //case iter->child  2
                //find successor
                lab2_node * leaf = iter->right;
                lab2_node * pleaf = iter;
                while(leaf->left!=NULL){
                    pleaf = leaf;
                    leaf = leaf->left;
                }
                //find suc = leaf
                iter->key = leaf->key;
                if (pleaf != iter) pleaf->left = NULL; //loop 를 돌긴함. 
                else pleaf->right = NULL; //loop안돔, 
                free(leaf); leaf = NULL;
            }
            else if (iter->left == NULL && iter->right == NULL){
                //case iter -> child 0
                if(iter == tree->root){
                    //Tree->root = child;
                    tree->root = NULL;
                    free(iter); iter = NULL;
                } 
                else{// case General
                    if(p_iter->value < iter->value)
                        p_iter -> right =NULL;
                    else 
                        p_iter -> left = NULL;
                    
                    free(iter); iter = NULL;
                }
            }
            else if(iter->left == NULL){
                //case iter->right 1 
                if(iter == tree->root){
                    tree->root = iter->right;
                    iter->right->parent = NULL;
                    free(iter); iter=NULL;
                }
                else{
                    p_iter->right = iter->right;
                    iter->right->parent = p_iter;
                    free(iter); iter=NULL; 
                }
            }
            else{
                //case iter->left 1
                if(iter == tree->root){
                    tree->root = iter->left;
                    iter->left->parent = NULL;
                    free(iter); iter=NULL;
                }
                else{
                    p_iter->left = iter->left;
                    iter->left->parent = p_iter;
                    free(iter); iter=NULL; 
                }
            }
        }
    }
    /*Critical Section END*/
    rwlock_release_writelock(tree->rw);
    return ret;
}
// reader_critical()
// writer_critical()
// _reader_safe(){}