
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "include/lab2_sync_types.h"
#include "include/rwlock.h"

void rw_bst_test(int num_threads,int node_count){

    lab2_tree *tree;
    lab2_node *node;    
    struct timeval tv_insert_start, tv_insert_end, tv_delete_start, tv_delete_end, tv_start, tv_end;
    int errors,i=0,count=0;
    int root_data = 40; 
    int term = node_count / num_threads, is_sync;
    double exe_time=0.0;
    thread_arg *threads;
    int *data = (int*)malloc(sizeof(int)*node_count);
    //open two file 1. Insert.csv
    // char * buf = (char *)malloc(sizeof(char )* 64);
    // int fd_insert = open("./insert.csv", O_WRONLY|O_APPEND);
    // if(fd_insert <0) fd_insert = open("./insert.csv", O_CREAT|O_RDWR , 0664);
    //     if(fd_insert<0) write(STDERR_FILENO, "Insert.csv Error\n\0", 24);
    // //2. Delete.scv
    // int fd_remove = open("./remove.csv", O_WRONLY|O_APPEND);
    // if(fd_remove <0) fd_remove = open("./remove.csv", O_CREAT|O_RDWR , 0664);
    // if(fd_remove <0) write(STDERR_FILENO, "delete.csv Error\n\0", 24);

    srand(time(NULL));
    for (i=0; i < node_count; i++) { 
        data[i] = rand();
    }

    if (!(threads = (thread_arg*)malloc(sizeof(thread_arg) * num_threads)))
        abort();

    /*
     * single thread insert test.
     */
    gettimeofday(&tv_start, NULL);
    printf("\n");
    tree = lab2_tree_create();
    for (i=0 ; i < node_count ; i++) {               
        lab2_node* node = lab2_node_create(data[i]);
        lab2_node_insert(tree, node);
    }

    gettimeofday(&tv_end, NULL);
    exe_time = get_timeval(&tv_start, &tv_end);
    print_result(tree,num_threads, node_count, LAB2_TYPE_SINGLE,LAB2_OPTYPE_INSERT ,exe_time);
    //lab2_tree_delete(tree);

    /* 
     * multi therad insert test coarse-grained 
     */
    is_sync = LAB2_TYPE_COARSEGRAINED;
    tree = lab2_tree_create();

    gettimeofday(&tv_insert_start, NULL);
    for(i=0; i < num_threads ; i++){
        thread_arg *th_arg = &threads[i];
        th_arg->tree = tree;
        th_arg->is_sync = is_sync;
        th_arg->data_set = data;
        th_arg->start = i*term;
        th_arg->end = (i+1)*term;

        pthread_create(&threads[i].thread,NULL,thread_job_insert,(void*)th_arg);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i].thread, NULL);

    gettimeofday(&tv_insert_end, NULL);
    exe_time = get_timeval(&tv_insert_start, &tv_insert_end);
    print_result(tree,num_threads, node_count, is_sync,LAB2_OPTYPE_INSERT ,exe_time);
    // sprintf(buf, "%d,%d,%d,%lf, \n",is_sync, num_threads, node_count, exe_time );
    // write(fd_insert, buf, strlen(buf));
    // buf[0] = '\0';
    lab2_tree_delete(tree);

    //Tree created

    

    tree = lab2_tree_create();
    for (i=0 ; i < node_count ; i++) {               
        lab2_node* node = lab2_node_create(data[i]);
        lab2_node_insert(tree, node);
    }

    gettimeofday(&tv_start, NULL);
    for(i=0 ; i < node_count ; i++){
        lab2_node_remove(tree,data[i]);
    }

    gettimeofday(&tv_end, NULL);
    exe_time = get_timeval(&tv_start, &tv_end);
    print_result(tree ,num_threads, node_count, LAB2_TYPE_SINGLE, LAB2_OPTYPE_DELETE,exe_time);
    lab2_tree_delete(tree);
    
    /* 
     * multi thread delete test coarse-grained  
     */
    is_sync = LAB2_TYPE_COARSEGRAINED;
    tree = lab2_tree_create();

    for (i=0; i < node_count; i++) { 
        node = lab2_node_create(data[i]);
        if(is_sync == LAB2_TYPE_FINEGRAINED)
            lab2_node_insert_fg(tree,node);
        else if(is_sync == LAB2_TYPE_COARSEGRAINED)
            lab2_node_insert_cg(tree,node);
    }            
    
    gettimeofday(&tv_delete_start, NULL);
    for(i=0 ; i < num_threads ; i++){
        thread_arg *th_arg = &threads[i];
        th_arg->tree = tree;
        th_arg->is_sync = is_sync;
        th_arg->data_set = data;
        th_arg->start = i*term;
        th_arg->end = (i+1)*term;

        pthread_create(&threads[i].thread,NULL,thread_job_delete,(void*)th_arg);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i].thread, NULL);
    gettimeofday(&tv_delete_end, NULL);
    exe_time = get_timeval(&tv_delete_start, &tv_delete_end);

    print_result(tree,num_threads, node_count, is_sync,LAB2_OPTYPE_DELETE,exe_time);
    // sprintf(buf, "%d,%d,%d,%lf \n",is_sync, num_threads, node_count, exe_time );
    // write(fd_remove, buf, strlen(buf));
    // buf[0] = '\0';
    lab2_tree_delete(tree);

    /* 
     * multi thread delete test fine-grained  
     */
    is_sync = LAB2_TYPE_FINEGRAINED;
    tree = lab2_tree_create();
    for (i=0; i < node_count; i++) { 
        node = lab2_node_create(data[i]);
        if(is_sync == LAB2_TYPE_FINEGRAINED)
            lab2_node_insert_fg(tree,node);
        else if(is_sync == LAB2_TYPE_COARSEGRAINED)
            lab2_node_insert_cg(tree,node);
    }
    lab2_node_init_inorder(tree);
    gettimeofday(&tv_delete_start, NULL);
    for(i=0 ; i < num_threads ; i++){
        thread_arg *th_arg = &threads[i];
        th_arg->tree = tree;
        th_arg->is_sync = is_sync;
        th_arg->data_set = data;
        th_arg->start = i*term;
        th_arg->end = (i+1)*term;

        pthread_create(&threads[i].thread,NULL,thread_job_delete,(void*)th_arg);
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i].thread, NULL);

    gettimeofday(&tv_delete_end, NULL);
    exe_time = get_timeval(&tv_delete_start, &tv_delete_end);

    print_result(tree ,num_threads, node_count, is_sync, LAB2_OPTYPE_DELETE,exe_time);
    // sprintf(buf, "%d,%d,%d,%lf, \n",is_sync, num_threads, node_count, exe_time );
    // write(fd_remove, buf, strlen(buf));
    // buf[0] = '\0';
    lab2_tree_delete(tree);

    printf("\n");
    // close(fd_insert);
    // close(fd_remove);
    free(threads);
    free(data);
}
//1. 20 Reader - 80Writer
//2. 50 Reader - 50Writer
//3. 80 Reader - 20Writer
