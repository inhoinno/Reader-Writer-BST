#include "include/lab2_sync_types.h"

void rwlock_init(rwlock_t *rw){
    Sem_init(&(rw->lock), 1);
    Sem_init(&(rw->writelock),1);
    Sem_init(&(rw->turnstile),1);
    rw->readers = 0;
    rw->writers = 0; 
};

//Read LOCK
void rwlock_acquire_readlock(rwlock_t *rw){
    Sem_wait(&(rw->turnstile));
    Sem_post(&(rw->turnstile));

    Sem_wait(&(rw->lock));
    rw->readers++;
    if(rw->readers ==1)
        Sem_wait(&(rw->writelock));
    
    Sem_post(&(rw->lock));
}


void rwlock_acquire_readlock(rwlock_t *rw){
    Sem_wait(&(rw->lock));
    rw->readers--;
    if(rw->readers == 0)
        Sem_post(&(rw->writelock));
    Sem_post(&(rw->lock));
}

//Write LoCK
void rwlock_acquire_writelock(rwlock_t *rw){
    Sem_wait(&(rw->turnstile));
    Sem_wait(&(rw->writelock));
    rw->writers++;
}

void rwlock_release_writelock(rwlock_t *rw){
    rw->writers --;
    Sem_post(&(rw->turnstile));
    Sem_post(&(rw->writelock));
}

