/*
*	DKU Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 
*	    Student name : 
*
*   lab1_sched.c :
*       - Lab2 header file.
*       - It contains bst node structure, timeval functions, atomic functions .. etc.
*
*/


#ifndef _LAB2_HEADER_H
#define _LAB2_HEADER_H

#include <pthread.h>
#include "rwlock.h"
#define LAB2_SUCCESS                0
#define LAB2_ERROR                 -1

/*

내가 보고싶은게 뭐지? -> 

Common BST performance(lab2 Coarse grain) vs 'RW BST'(Coarse grain) performance 

RW - BST 중에서는
    Common RW BST vs RW-writer-priority BST performance  
    Optimal BST vs Optimal RW BST

어떤 그림을 예상하고 있지? -> 두 기법의 동일 워크로드 (YCSB, levelDB Benchmark)등에서 성능 측정
동일 시간동안 Reader 의 Throughput

*/

/*
 * lab2_node
 *
 *  struct lab2_node *left  : left child link
 *  struct lab2_node *right : right child link
 *  int key                 : node key value 
 */
typedef struct lab2_node {

    pthread_mutex_t mutex;    
    //pthread_mutex_t mutex_left;
    //pthread_mutex_t mutex_right;

    struct lab2_node *left;
    struct lab2_node *right;
    struct lab2_node *parent;
    volatile int key;
    volatile int value;
    volatile int level;
} lab2_node;

/*
 * lab2_tree
 *
 *  struct lab2_node *root  : root node of bst.
 */
typedef struct lab2_tree {
    struct lab2_node *root;
    rwlock_t *rw;
    sem_t * cglock;
} lab2_tree;

/* 
 * lab2_bst_test.c related structure.  
 */
typedef struct thread_arg{

    pthread_t thread;
    lab2_tree *tree;
    int node_count;
    int num_iterations;
    int is_sync;
    int *data_set;
    int start;
    int end;

}thread_arg;


/*
 * 
 * LOCK : if we try to init lock by tree-level-scope , 
 * then we need may need RW_lockList
 *  
*/

/* 
 * lab2_bst.c related functions 
 * TODO
 *  You need to implement these functions. 
 */
int lab2_node_print_inorder(lab2_tree *tree);
int lab2_node_print(lab2_node * node);
lab2_tree *lab2_tree_create();
lab2_node *lab2_node_create(int key);
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node);
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node);
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node);
int lab2_node_remove(lab2_tree *tree, int key);
int lab2_node_remove_fg(lab2_tree *tree, int key);
int lab2_node_remove_cg(lab2_tree *tree, int key);
void lab2_tree_delete(lab2_tree *tree);
void lab2_node_delete(lab2_node *node);
int lab2_node_init_inorder(lab2_tree *tree);
int lab2_node_init(lab2_node *node);

int Reader(lab2_tree *tree, int val, rwlock_t *rw);
int searcher(lab2_node *node , int value);
lab2_node* _searcher_node(lab2_node *node , int value);
lab2_node* _searcher_parent_node(lab2_node *node , int value);
int Writer_insert_cg(lab2_tree *tree, lab2_node * node);
int Writer_delete_cg(lab2_tree *tree, int val);




/*
 * Lab2 bonus(lab2_bonus.c)
 *  Spin Lock implementation in assembly level. 
 *
 * lab2_bonus.c related functions  
 */

/* assembly implementation example functions */
void atomic_add( int * value, int inc_val);
void atomic_sub( int * value, int dec_val);
void atomic_inc( int * value);
void atomic_dec( int * value);

/*
 * lab2_bonus.c related functions 
 * TODO
 *  You need to implement these function.
 */
void lab2_spin_lock(int volatile *lock);
void lab2_spin_unlock(int volatile *lock);


/* lab2_timeval.c related function */
double get_timeval(struct timeval *tv, struct timeval *tv_end);

#endif /* LAB2_HEADER_H*/
