
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

void unit_test(int count){
    int i= 0;
    int counter = 0;
    sem_t CG;
    sem_t CG2;
    rwlock_t RW;
    Sem_init(&CG, 1);
    Sem_init(&CG2, 1);
    rwlock_init(&RW);
    struct timeval tv_insert_start, tv_insert_end, tv_delete_start, tv_delete_end, tv_start, tv_end;
    struct timeval rw_start, rw_end;
    double execution_time = 0.0; 

    gettimeofday(&tv_delete_start, NULL);
    for(i = 0; i <count; i++){
        rwlock_acquire_writelock(&RW);
        counter++;
        rwlock_release_writelock(&RW);
    }
    gettimeofday(&tv_delete_end, NULL);
    execution_time = get_timeval(&tv_delete_start, &tv_delete_end);
    printf("======= RW writer Total EXE : %lf ========\n", execution_time);
    printf("=======     Total Counter   : %d ========\n", counter);
    counter = 0;

    gettimeofday(&tv_delete_start, NULL);
    for(i = 0; i < count; i++){
        rwlock_acquire_readlock(&RW);
        counter++;
        rwlock_release_readlock(&RW);
    }
    gettimeofday(&tv_delete_end, NULL);
    execution_time = get_timeval(&tv_delete_start, &tv_delete_end);
    printf("======= RW reader Total EXE : %lf ========\n", execution_time);
    printf("=======     Total Counter   : %d  ========\n", counter);
    counter = 0;
    
    gettimeofday(&tv_delete_start, NULL);
    for(i = 0; i < count; i++){
        Sem_wait(&CG);
        counter++;
        Sem_post(&CG);
    }
    gettimeofday(&tv_delete_end, NULL);
    execution_time = get_timeval(&tv_delete_start, &tv_delete_end);

    printf("======= Single sem_t Total EXE : %lf ========\n", execution_time);
    printf("=======     Total Counter      : %d  ========\n", counter);
    counter = 0;

    Sem_init(&CG, 2);

    gettimeofday(&tv_delete_start, NULL);
    for(i = 0; i < count; i++){
        Sem_wait(&CG);
        Sem_wait(&CG);
        counter++;
        Sem_post(&CG);
        Sem_post(&CG);
    }
    gettimeofday(&tv_delete_end, NULL);
    execution_time = get_timeval(&tv_delete_start, &tv_delete_end);

    printf("======= Double sem_t Total EXE : %lf ========\n", execution_time);
    printf("=======     Total Counter      : %d  ========\n", counter);
    counter = 0;

    Sem_init(&CG, 1);
    gettimeofday(&tv_delete_start, NULL);
    for(i = 0; i < count; i++){
        Sem_wait(&CG);
        Sem_wait(&CG2);
        counter++;
        Sem_post(&CG);
        Sem_post(&CG2);
    }
    gettimeofday(&tv_delete_end, NULL);
    execution_time = get_timeval(&tv_delete_start, &tv_delete_end);

    printf("======= Double sem_t Total EXE : %lf ========\n", execution_time);
    printf("=======     Total Counter      : %d  ========\n", counter);
    counter = 0;

}

int main(int argc, char* argv[]){
    printf("::::      UNIT TEST START     ::::\n");
    

    char op;
    int num_threads=0, node_count=0;
    char* redir = "";
    int fd;

    optind = 0;

    while ((op = getopt(argc, argv, "c:r:")) != -1) {
        switch (op) {
            case 'c':
                node_count = atoi(optarg);
                break;
            case 'r':
                redir = (char*)optarg;

            default:
                break;
        }
    }
    printf("redir : %s\n",redir);
    if((node_count > 0)){
        unit_test(node_count);
    }

    return LAB2_SUCCESS;
}