#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common_threads.h"

//
// Your code goes in the structure and functions below
//

typedef struct __rwlock_t {
    dispatch_semaphore_t readlock;
    dispatch_semaphore_t writelock;
    dispatch_semaphore_t writer_waitinglock;
    size_t num_reader;
} rwlock_t;


void rwlock_init(rwlock_t *rw) {
    Sem_init(&(rw->readlock), 1);
    Sem_init(&(rw->writelock), 1);
    Sem_init(&(rw->writer_waitinglock), 1);
    rw->num_reader = 0;
}

void rwlock_acquire_readlock(rwlock_t *rw) {
    Sem_wait(&(rw->writer_waitinglock));
    Sem_post(&(rw->writer_waitinglock));
    Sem_wait(&(rw->readlock));
    if (rw->num_reader == 0) {
        Sem_wait(&(rw->writelock));
    }
    rw->num_reader++;
    Sem_post(&(rw->readlock));
}

void rwlock_release_readlock(rwlock_t *rw) {
    Sem_wait(&(rw->readlock));
    if (--(rw->num_reader) == 0)
        Sem_post(&(rw->writelock));
    Sem_post(&(rw->readlock));
}

void rwlock_acquire_writelock(rwlock_t *rw) {
    Sem_wait(&(rw->writer_waitinglock));
    Sem_wait(&(rw->writelock));
}

void rwlock_release_writelock(rwlock_t *rw) {
    Sem_post(&(rw->writelock));
    Sem_post(&(rw->writer_waitinglock));
}

//
// Don't change the code below (just use it!)
// 

int loops;
int value = 0;

rwlock_t lock;

void *reader(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_readlock(&lock);
	printf("read %d\n", value);
	rwlock_release_readlock(&lock);
    }
    return NULL;
}

void *writer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	rwlock_acquire_writelock(&lock);
	value++;
	printf("write %d\n", value);
	rwlock_release_writelock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 4);
    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);
    loops = atoi(argv[3]);

    pthread_t pr[num_readers], pw[num_writers];

    rwlock_init(&lock);

    printf("begin\n");

    int i;
    for (i = 0; i < num_readers; i++)
	Pthread_create(&pr[i], NULL, reader, NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_create(&pw[i], NULL, writer, NULL);

    for (i = 0; i < num_readers; i++)
	Pthread_join(pr[i], NULL);
    for (i = 0; i < num_writers; i++)
	Pthread_join(pw[i], NULL);

    printf("end: value %d\n", value);

    return 0;
}

