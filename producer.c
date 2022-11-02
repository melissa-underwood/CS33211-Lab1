#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int main() {
    const char *name      = "shared memory";
    const char *sem_full  = "full" ;
    const char *sem_mutex = "mutex";
    const char *sem_empty = "empty";

    sem_t *full  = sem_open(sem_full,  O_CREAT, 7026, 0);
    sem_t *mutex = sem_open(sem_mutex, O_CREAT, 7026, 1);
    sem_t *empty = sem_open(sem_empty, O_CREAT, 7026, 2);

    // creating shared memory
    int items  = 2;
    int shdmem = shm_open(name, O_RDONLY | O_EXCL | O_RDWR, items);
    ftruncate(shdmem, sizeof(int));

    // map object into caller's address space
    int *table = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shdmem, 0);

    int rounds = 5;
    do {
        /* produce an item in next_produced */
        sem_wait(empty);
        sem_wait(mutex);
        /* add next produced to the buffer */
        (*table)++; 
        printf("Item was produced. \n");
        sem_post(mutex);
        sem_post(full);
    } while (rounds--);

    shm_unlink(name);

    exit(EXIT_SUCCESS);
}