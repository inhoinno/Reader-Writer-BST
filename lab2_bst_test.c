/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*
*   lab2_bst_test.c :
*       - thread-safe bst test code.
*       - coarse-grained, fine-grained lock test code
*
* You can compare single thread result, coarse grained result and fine grained result.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "include/rwlock.h"
#include "include/lab2_sync_types.h"

#define LAB2_TYPE_FINEGRAINED       0
#define LAB2_TYPE_COARSEGRAINED     1
#define LAB2_TYPE_SINGLE            2

#define LAB2_OPTYPE_INSERT          0
#define LAB2_OPTYPE_DELETE          1

void lab2_sync_usage(char *cmd)
{
    printf("\n Usage for %s : \n",cmd);
    printf("    -t: num thread, must be bigger than 0 ( e.g. 4 )\n");
    printf("    -c: test node count, must be bigger than 0 ( e.g. 10000000 ) \n");
}

void lab2_sync_example(char *cmd)
{
    printf("\n Example : \n");
    printf("    #sudo %s -t 4 -c 10000000 \n", cmd);
    printf("    #sudo %s -t 4 -c 10000000 \n", cmd);
}

static void print_result(lab2_tree *tree,int num_threads,int node_count ,int is_sync, int op_type ,double time){
    char *cond[] = {"fine-grained BST  ", "coarse-grained BST", "single thread BST"};
    char *op[] = {"insert","delete"};
    int result_count=0;


    printf("=====  Multi thread %s  %s experiment  =====\n",cond[is_sync],op[op_type]);
    printf("\n Experiment info \n");
    printf("    test node           : %d \n",node_count);
    printf("    test threads        : %d \n",num_threads);
    printf("    execution time      : %lf seconds \n\n",time);

    printf("\n BST inorder iteration result : \n");


}
void* thread_job_search(void *arg){

    thread_arg *th_arg = (thread_arg *)arg;
    lab2_tree *tree = th_arg->tree;
    int is_sync = th_arg->is_sync;
    int *data_set = th_arg->data_set;
    int start = th_arg->start, end = th_arg->end;
    int i;
    for(i=start ; i < end; i++ ){
        if(is_sync == LAB2_TYPE_FINEGRAINED)
            Reader(tree, data_set[i]);        
        else if(is_sync == LAB2_TYPE_COARSEGRAINED)
            lab2_node_search(tree, data_set[i]);
    }

}

void* thread_job_delete(void *arg){

    thread_arg *th_arg = (thread_arg *)arg;
    lab2_tree *tree = th_arg->tree;
    int is_sync = th_arg->is_sync;
    int *data_set = th_arg->data_set;
    int start = th_arg->start, end = th_arg->end;
    int i;
    for(i=start ; i < end; i++ ){
        if(is_sync == LAB2_TYPE_FINEGRAINED)
            Writer_delete_cg(tree, data_set[i]);        
        else if(is_sync == LAB2_TYPE_COARSEGRAINED)
            lab2_node_remove_cg(tree, data_set[i]);
    }
}

void* thread_job_insert(void *arg){

    thread_arg *th_arg = (thread_arg *)arg;
    lab2_tree *tree = th_arg->tree;
    
    int is_sync = th_arg->is_sync;
    int *data = th_arg->data_set;
    int start = th_arg->start, end = th_arg->end;
    int i;

    for (i=start ; i < end ; i++) {               
        lab2_node* node = lab2_node_create(data[i]);
        if(is_sync == LAB2_TYPE_FINEGRAINED)
            Writer_insert_cg(tree, node);
        else if(is_sync == LAB2_TYPE_COARSEGRAINED)
            lab2_node_insert_cg(tree, node);
    }
}

void bst_test(int num_threads,int node_count){

    lab2_tree *tree;
    lab2_node *node;    
    struct timeval tv_insert_start, tv_insert_end, tv_delete_start, tv_delete_end, tv_start, tv_end;
    struct timeval rw_start, rw_end;
    int errors,i=0,count=0;
    int root_data = 40; 
    int term = node_count / num_threads, is_sync;
    double exe_time=0.0;
    thread_arg *threads;
    int *data = (int*)malloc(sizeof(int)*node_count);
    //open two file 
    //1. CG ONLY.csv
    char * buf = (char *)malloc(sizeof(char )*100 );
    int fd_cg = open("./cgonly.csv", O_WRONLY|O_APPEND);
    if(fd_cg <0) fd_cg = open("./CGonly.csv", O_CREAT|O_RDWR|O_APPEND , 0664);
    if(fd_cg<0) write(STDERR_FILENO, "cg.csv Error\n\0", 24);
    // //2. Delete.scv
    int fd_rwcg = open("./rwcg.csv", O_WRONLY|O_APPEND);
    if(fd_rwcg <0) fd_rwcg = open("./rwcg.csv", O_CREAT|O_RDWR , 0664);
    if(fd_rwcg <0) write(STDERR_FILENO, "rwcg.csv Error\n\0", 24);

    //3. CG readers.csv
    
    int  fd_cg_r = open("./CGonly_reader.csv", O_CREAT|O_RDWR|O_APPEND , 0664);
    if(fd_cg_r <0) write(STDERR_FILENO, "cg_reader.csv Error\n\0", 24);

    // 4. RW readers.scv
    int fd_rwcg_r = open("./rwcg_reader.csv", O_CREAT|O_RDWR|O_APPEND , 0664);
    if(fd_rwcg_r <0) write(STDERR_FILENO, "rwcg_reader.csv Error\n\0", 24);

    srand(time(NULL));
    for (i=0; i < node_count; i++) { 
        data[i] = rand();
    }

    if (!(threads = (thread_arg*)malloc(sizeof(thread_arg) * num_threads)))
        abort();



    /* [WRITER ONLY - 단일 세마포어]
     * 1. multi thread insert-delete test coarse-grained  
     */
    is_sync = LAB2_TYPE_COARSEGRAINED;
    int coin = 0;
    int k =0;
    double total_exe_time = 0;
    for(k=0; k<25; k++){
    tree = lab2_tree_create();
	for (i=0; i < node_count/10; i++) { 
       		node = lab2_node_create((data[i]+(rand()/100)));
        	lab2_node_insert_cg(tree,node);
   	 }	            
 
    
	gettimeofday(&rw_start, NULL);
    for(i=0 ; i < num_threads ; i++){
        thread_arg *th_arg = &threads[i];
        th_arg->tree = tree;
        th_arg->is_sync = is_sync;
        th_arg->data_set = data;
        th_arg->start = i*term;
        th_arg->end = (i+1)*term;
        //if(reader_stride + reader_pass >= reader_stride + writer_pass){
            //pthread_create(&threads[i].thread,NULL,thread_job_delete,(void*)th_arg);
        //}else{
            if(coin){
                coin =0;
                pthread_create(&threads[i].thread,NULL,thread_job_insert,(void*)th_arg);
            }else{
                coin = 1;
                pthread_create(&threads[i].thread,NULL,thread_job_delete,(void*)th_arg);
            }
        //}
    }
    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i].thread, NULL);

    gettimeofday(&rw_end, NULL);
    exe_time = get_timeval(&rw_start, &rw_end);
    total_exe_time += exe_time;
    lab2_tree_delete(tree);
    }
    printf("[WRITER ONLY]\n");
    print_result(tree ,num_threads, node_count, is_sync, LAB2_OPTYPE_DELETE,exe_time);
    sprintf(buf, "%d,%d,%lf, \n", num_threads, node_count, (total_exe_time/(k-1)) );
    write(fd_cg, buf, strlen(buf));
    close(fd_cg); total_exe_time = 0;
    buf[0] = '\0';

    /* [WRITER ONLY - RWlock machanism]
     * 2. multi thread insert-delete test rwlock-grained  
     */
    is_sync = LAB2_TYPE_FINEGRAINED;

    for(k =0 ; k< 25; k++){
    tree = lab2_tree_create();
    	for (i=0; i < node_count/10; i++) { 
       		node = lab2_node_create((data[i]+(rand()/100)));
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
        //if(reader_stride + reader_pass >= reader_stride + writer_pass){
            //pthread_create(&threads[i].thread,NULL,thread_job_delete,(void*)th_arg);
        //}else{
            if(coin){
                coin =0;
                pthread_create(&threads[i].thread,NULL,thread_job_insert,(void*)th_arg);
            }else{
                coin = 1;
                pthread_create(&threads[i].thread,NULL,thread_job_delete,(void*)th_arg);
            }
        //}
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i].thread, NULL);

    gettimeofday(&tv_delete_end, NULL);
    exe_time = get_timeval(&tv_delete_start, &tv_delete_end);
    total_exe_time += exe_time;
    lab2_tree_delete(tree);
    }
    print_result(tree ,num_threads, node_count, is_sync, LAB2_OPTYPE_DELETE,exe_time);
    sprintf(buf, "%d,%d,%lf, \n", num_threads, node_count, (total_exe_time/(k-1)) );
    write(fd_rwcg, buf, strlen(buf));
    buf[0] = '\0'; total_exe_time=0;
    close(fd_rwcg);
	


	/* 
     * 3. multi thread insert-delete test coarse-grained  
     */
    is_sync = LAB2_TYPE_COARSEGRAINED;
	/*
	making a stride schedule to Reader-Writer
	*/    
	//TODO 3 : make argv[] to get p_reader, p_writer
    int p_reader = 20 ; // atoi(argv[?])	//reader_stride = 100 / 80 = 1
	int p_writer = 80 ; // atoi(argv[?])	//writer_stride = 100 / 20 = 5
    rwstride_t * rwstride = rw_stride_create_stride();
    rw_stride_init_stride(rwstride,p_reader,p_writer);

	/*testing for 25 times*/
    for(k =0 ; k< 25; k++){

    tree = lab2_tree_create();
	
 	gettimeofday(&tv_delete_start, NULL);

	for(i=0 ; i < num_threads ; i++){
        thread_arg *th_arg = &threads[i];
        th_arg->tree = tree;
        th_arg->is_sync = is_sync;
        th_arg->data_set = data;
        th_arg->start = i*term;
        th_arg->end = (i+1)*term;

        if(rw_stride_schedule(rwstride))
        {
            //COND : rw_stride_schedule(rw_stride) , RET = 1 to reader : 0 to writer
            pthread_create(&threads[i].thread,NULL,thread_job_search,(void*)th_arg);
        }else{
            
            if(coin){
                coin =0;
                pthread_create(&threads[i].thread,NULL,thread_job_insert,(void*)th_arg);
            }else{
                coin = 1;
                pthread_create(&threads[i].thread,NULL,thread_job_delete,(void*)th_arg);
            }
        }
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i].thread, NULL);

    gettimeofday(&tv_delete_end, NULL);
    exe_time = get_timeval(&tv_delete_start, &tv_delete_end);
    total_exe_time += exe_time;
    lab2_tree_delete(tree);
    }
    printf("[READER-WRITER]\n");
    print_result(tree ,num_threads, node_count, is_sync, LAB2_OPTYPE_DELETE,exe_time);
    sprintf(buf, "%d,%d,%lf, \n", num_threads, node_count, (total_exe_time/(k-1)) );
    write(fd_cg_r, buf, strlen(buf));
    close(fd_cg_r);
    buf[0] = '\0'; 
    total_exe_time=0;


	/* 
     * 4. multi thread reader_writer test rwlock-grained  
     */
    is_sync = LAB2_TYPE_FINEGRAINED;
	/*
	making a stride schedule to Reader-Writer
	*/    
	//TODO 1 : make RW_Stride DataStructure that schedule reader-wrtier in stride scheduling
	//struct * rw_stride = create_stride();
	//init_stride(rw_stride, p_reader,p_writer); 
 
	rw_stride_init_stride(rwstride, p_reader, p_writer);

	/*testing for 25 times*/
    for(k =0 ; k< 25; k++){

        tree = lab2_tree_create();
        gettimeofday(&tv_delete_start, NULL);
        //Tic

        for(i=0 ; i < num_threads ; i++){
            thread_arg *th_arg = &threads[i];
            th_arg->tree = tree;
            th_arg->is_sync = is_sync;
            th_arg->data_set = data;
            th_arg->start = i*term;
            th_arg->end = (i+1)*term;
            if(rw_stride_schedule(rwstride))
            {
                //COND : rw_stride_schedule(rw_stride) , RET = 1 to reader : 0 to writer
                pthread_create(&threads[i].thread,NULL,thread_job_search,(void*)th_arg);
            }else{
                
                if(coin){
                    coin =0;
                    pthread_create(&threads[i].thread,NULL,thread_job_insert,(void*)th_arg);
                }else{
                    coin = 1;
                    pthread_create(&threads[i].thread,NULL,thread_job_delete,(void*)th_arg);
                }
            }
        }

        for (i = 0; i < num_threads; i++)
            pthread_join(threads[i].thread, NULL);

        gettimeofday(&tv_delete_end, NULL);
        //Toc
        exe_time = get_timeval(&tv_delete_start, &tv_delete_end);
        total_exe_time += exe_time;
        lab2_tree_delete(tree);
    }
    print_result(tree ,num_threads, node_count, is_sync, LAB2_OPTYPE_DELETE,exe_time);
    sprintf(buf, "%d,%d,%lf, \n", num_threads, node_count, (total_exe_time/(k-1)) );
    write(fd_rwcg_r, buf, strlen(buf));
    buf[0] = '\0'; 
    total_exe_time=0;
    close(fd_rwcg_r);


    printf("\n");

    free(threads);
    free(data);
}
//1. 20 Reader - 80Writer
//2. 50 Reader - 50Writer
//3. 80 Reader - 20Writer

int main(int argc, char *argv[]) 
{
    char op;
    int num_threads=0, node_count=0;
    int fd;

    optind = 0;

    while ((op = getopt(argc, argv, "t:c:")) != -1) {
        switch (op) {
            case 't':
                num_threads=atoi(optarg);
                break;
            case 'c':
                node_count = atoi(optarg);
                break;
            default:
                goto INVALID_ARGS;
        }
    }
    if((num_threads>0) && (node_count > 0)){
        bst_test(num_threads,node_count);
    }else{
        goto INVALID_ARGS;
    }

    return LAB2_SUCCESS;
INVALID_ARGS:
    lab2_sync_usage(argv[0]);
    lab2_sync_example(argv[0]);

    return LAB2_ERROR;
}

