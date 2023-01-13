#include <thread_realization.h>
#include <preparation.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define TASK_COUNT 10

task* single_thread_run(int fd) {
    struct stat file_stat = {};

    if (fstat(fd, &file_stat) == -1) {
        perror("fstat");
        return NULL;
    }

    int* mapped_file = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (mapped_file == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }

    task* tasks_pull = malloc(sizeof(task) * TASK_COUNT);

    if (!tasks_pull) {
        perror("malloc");
        return NULL;
    }

    for (int i = 0; i < TASK_COUNT; i++) {
        tasks_pull[i].sum = 0;
    }

    for (int j = 0; j < FILE_SIZE; j++) {
        tasks_pull[j % TASK_COUNT].sum += mapped_file[j];
    }

    if (munmap(mapped_file, file_stat.st_size)) {
        perror("munmap");
        free(tasks_pull);
        return NULL;
    }

    return tasks_pull;
}
