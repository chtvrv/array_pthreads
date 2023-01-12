#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>

#define FILE_SIZE 100000000
#define TASK_COUNT 10

struct Task {
    int start_pos;
    int sum;
};

typedef struct Task task;

bool prep_test_files();
bool create_test_files(FILE **file_buf);
bool fill_test_files(FILE **file_buf);
int *open_test_files();
int close_test_files(int *fds);

#ifdef __cplusplus
}
#endif
