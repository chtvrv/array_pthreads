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

static int job(int *buf, int start_pos) {
    int result = 0;

    for (int i = start_pos; i < FILE_SIZE; i += 10) {
        result += buf[i];
    }

    return result;
}

task *single_thread_run(int fd) {
    struct stat file_stat = {};

    if (fstat(fd, &file_stat) == -1) {
        perror("fstat");
        return NULL;
    }

    int *file_in_memory = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (file_in_memory == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }

    task *task_pull = malloc(sizeof(task) * TASK_COUNT);

    if (!task_pull) {
        perror("malloc");
        return NULL;
    }

    for (int i = 0; i < TASK_COUNT; i++) {
        task_pull[i].start_pos = i;

        int result = job(file_in_memory, i);

        task_pull[i].sum = result;
    }

    if (munmap(file_in_memory, file_stat.st_size)) {
        perror("munmap");
        free(task_pull);
        return NULL;
    }

    return task_pull;
}
