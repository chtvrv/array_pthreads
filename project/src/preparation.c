#define _POSIX_C_SOURCE 199506L

#include <stdio.h>
#include <preparation.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


static const int files_count = 3;

static const char *test_file_with_zeros = "zeros_filled.bin";
static const char *test_file_with_mix = "mix_filled.bin";
static const char *test_file_with_rand = "rand_filled.bin";

static const int chunk_size = 10000;

static const int chunks_in_file = (FILE_SIZE / 10000);

bool prep_test_files() {
    FILE **file_buf = malloc(sizeof(FILE *) * files_count);

    if (!file_buf) {
        return false;
    }

    bool ready_state = create_test_files(file_buf);

    if (!ready_state) {
        free(file_buf);
        return false;
    }

    bool files_filled = fill_test_files(file_buf);

    if (files_filled) {
        printf("\nSUCCESS FILES FILL\n\n");
    } else {
        printf("\nFAILED FILES FILL\n\n");
    }

    for (int i = 0; i < files_count; i++) {
        fclose(file_buf[i]);
    }

    free(file_buf);

    return true;
}

bool create_test_files(FILE **file_buf) {
    assert(file_buf);

    file_buf[0] = fopen(test_file_with_zeros, "w+");
    file_buf[1] = fopen(test_file_with_mix, "w+");
    file_buf[2] = fopen(test_file_with_rand, "w+");

    bool ready_state = (file_buf[0] && file_buf[1] && file_buf[2]);

    if (!ready_state) {
        for (int i = 0; i < files_count; i++) {
            if (file_buf[i]) {
                fclose(file_buf[i]);
            }
        }

        return false;
    }

    return true;
}

bool fill_test_files(FILE **file_buf) {
    assert(file_buf);

    int *buf_for_copy = malloc(sizeof(int) * chunk_size);

    if (!buf_for_copy) {
        return false;
    }

    unsigned int seed_rand = 0;

    for (int k = 0; k < files_count; k++) {
        FILE *current_file = file_buf[k];

        for (int i = 0; i < chunks_in_file; i++) {
            for (int j = 0; j < chunk_size; j++) {
                if (k == 0) {
                    buf_for_copy[j] = 0;
                }

                if (k == 1) {
                    buf_for_copy[j] = j % 10;
                }

                if (k == 2) {
                    buf_for_copy[j] = rand_r(&seed_rand) % 1000;
                }
            }

            int write_result = fwrite(buf_for_copy, sizeof(int), chunk_size, current_file);

            if (write_result != chunk_size) {
                perror("fwrite");
                free(buf_for_copy);
                return false;
            }
        }
    }

    free(buf_for_copy);
    return true;
}


int *open_test_files() {
    int *file_buf = malloc(sizeof(int) * files_count);

    if (!file_buf) {
        return NULL;
    }

    file_buf[0] = open(test_file_with_zeros, O_RDONLY, S_IRUSR | S_IWUSR);
    file_buf[1] = open(test_file_with_mix, O_RDONLY, S_IRUSR | S_IWUSR);
    file_buf[2] = open(test_file_with_rand, O_RDONLY, S_IRUSR | S_IWUSR);

    bool ready_state = (file_buf[0] && file_buf[1] && file_buf[2]);

    if (!ready_state) {
        for (int i = 0; i < files_count; i++) {
            if (file_buf[i]) {
                close(file_buf[i]);
            }
        }

        free(file_buf);
        return NULL;
    }

    return file_buf;
}


int close_test_files(int *fds) {
    int check_flag = 0;

    for (int i = 0; i < files_count; i++) {
        check_flag = close(fds[i]);
    }

    if (check_flag) {
        free(fds);
        return 1;
    }

    free(fds);
    return 0;
}
