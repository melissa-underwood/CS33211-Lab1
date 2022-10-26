#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    const char *name = "shared memory";
    int shdmem = shm_open(name, O_RDONLY | O_RDWR);


    shm_unlink(name);
}