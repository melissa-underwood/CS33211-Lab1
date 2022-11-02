#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

int main() {
    const char *name      = "shared memory";
    const char *sem_full  = "full" ;
    const char *sem_mutex = "mutex";
    const char *sem_empty = "empty";

    sem_t *full  = sem_open(sem_full,  O_CREAT, 7026, 0);
    sem_t *mutex = sem_open(sem_mutex, O_CREAT, 7026, 1);
    sem_t *empty = sem_open(sem_empty, O_CREAT, 7026, 2);

    int shdmem = shm_open(name, O_RDWR, 7026);
    int *table = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shdmem, 0);

    int rounds = 5;
    do {
        sem_wait(full);
        sem_wait(mutex);
        /* remove an item from buffer */
        (*table)--;
        sem_post(mutex);
        sem_post(empty);
        printf("Item was consumed. \n");
    } while (rounds--);

    shm_unlink(name);

    exit(EXIT_SUCCESS);
}