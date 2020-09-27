#include "rwlock.h"

void rwlock_init(rwlock_t *rw) {
        Sem_init(&(rw->lock), 1);
        Sem_init(&(rw->writelock),1);
        Sem_init(&(rw->turnstile),1);
        rw->readers =0;
        rw->writers = 0;

}

void rwlock_acquire_readlock(rwlock_t *rw) {
        Sem_wait(&(rw->turnstile));
        Sem_post(&(rw->turnstile));

                Sem_wait(&(rw->lock));
                rw->readers++;
                if(rw->readers == 1)
                        Sem_wait(&(rw->writelock));
                //sleep(1);
                Sem_post(&(rw->lock));
}

void rwlock_release_readlock(rwlock_t *rw) {
        Sem_wait(&(rw->lock));
        rw->readers--;
        if(rw->readers == 0)
                Sem_post(&(rw->writelock));
        Sem_post(&(rw->lock));
}

void rwlock_acquire_writelock(rwlock_t *rw) {
        Sem_wait(&(rw->turnstile));
        Sem_wait(&(rw->writelock));
        rw->writers++;
        //sleep(1);
}

void rwlock_release_writelock(rwlock_t *rw) {
        rw->writers--;
        Sem_post(&(rw->turnstile));
        Sem_post(&(rw->writelock));
}

