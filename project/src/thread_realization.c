#include <thread_realization.h>
#include <preparation.h>
#include <pthread.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define TASK_COUNT 10

typedef struct thread_args {
    int offset;
    int *mmap_file_ptr;
    int chunk_size;
} thread_args;


static void *thread_func(void *arg);

task* multi_thread_run(int fd) {
    // int number_of_cores = sysconf(_SC_NPROCESSORS_ONLN);
    int number_of_cores = 8;

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

    pthread_t* workers_pull = malloc(sizeof(pthread_t) * number_of_cores);

    if (!worker_pull) {
        perror("malloc");
        return NULL;
    }

    task* tasks_pull = malloc(sizeof(task) * TASK_COUNT);

    if (!tasks_pull) {
        perror("malloc");
        free(workers_pull);
        return NULL;
    }

    for (int i = 0; i < TASK_COUNT; i++) {
        tasks_pull[i].start_pos = i;
        tasks_pull[i].sum = 0;
    }

    thread_args* args = malloc(sizeof(thread_args) * number_of_cores);

    for (int i = 0; i < number_of_cores; i++) {
        args[i].offset = i * (FILE_SIZE / number_of_cores);
        args[i].mmap_file_ptr = mapped_file;
        args[i].chunk_size = (FILE_SIZE / number_of_cores);
    }

    for (int i = 0; i < number_of_cores; i++) {
        if (pthread_create(&workers_pull[i], NULL, thread_func, (void *) &args[i])) {
            perror("pthread_create");
        }
    }

    for (int i = 0; i < number_of_cores; i++) {
        task* worker_task = NULL;

        pthread_join(workers_pull[i], (void *) &worker_task);

        for (int j = 0; j < TASK_COUNT; j++) {
            tasks_pull[j].sum += worker_task[j].sum;
        }

        free(worker_task);
    }

    free(args);
    free(worker_pull);

    munmap(file_in_memory, file_stat.st_size);
    return result_task_pull;
}

static void *thread_func(void *arg) {
    thread_args args = *(thread_args*) arg;

    task* worker_tasks = malloc(sizeof(task) * TASK_COUNT);

    for (int i = 0; i < TASK_COUNT; i++) {
        worker_tasks[i].start_pos = i;
        worker_tasks[i].sum = 0;
    }

    int* buf = args.mmap_file_ptr;
    int offset = args.offset;
    int finish = offset + args.chunk_size;

    for (int i = offset; i < finish; i++) {
        worker_tasks[i % TASK_COUNT].sum += buf[i];
    }

    pthread_exit((void *) worker_tasks);
}
