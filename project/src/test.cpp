#define _POSIX_C_SOURCE 199309L

#include <preparation.h>
#include <thread_realization.h>
#include <single_realization.h>
#include <gtest/gtest.h>
#include <time.h>

static int *fds_buf = NULL;

static bool task_compare(const task *A, const task *B) {
    if (A->start_pos != B->start_pos) {
        return false;
    }

    if (A->sum != B->sum) {
        return false;
    }

    return true;
}


TEST(preparation, create_test_files) {
    bool prep_state = prep_test_files();

    ASSERT_EQ(prep_state, true);
}

TEST(preparation, open_test_files) {
    fds_buf = open_test_files();

    ASSERT_TRUE(fds_buf != NULL);
}

TEST(single_test, zeros_bin) {
    task expected_result[TASK_COUNT] = {
        {0, 0}, {1, 0},
        {2, 0}, {3, 0},
        {4, 0}, {5, 0},
        {6, 0}, {7, 0},
        {8, 0}, {9, 0}
    };

    task *received_result = single_thread_run(fds_buf[0]);

    bool are_equal = true;

    for (int i = 0; i < TASK_COUNT; i++) {
        are_equal = task_compare(&expected_result[i], &received_result[i]);

        if (!are_equal) {
            break;
        }
    }

    free(received_result);

    EXPECT_EQ(are_equal, true);
}

TEST(thread_test, zeros_bin) {
    task expected_result[TASK_COUNT] = {
        {0, 0}, {1, 0},
        {2, 0}, {3, 0},
        {4, 0}, {5, 0},
        {6, 0}, {7, 0},
        {8, 0}, {9, 0}
    };

    task *received_result = multi_thread_run(fds_buf[0]);

    bool are_equal = true;

    for (int i = 0; i < TASK_COUNT; i++) {
        are_equal = task_compare(&expected_result[i], &received_result[i]);

        if (!are_equal) {
            break;
        }
    }

    free(received_result);

    EXPECT_EQ(are_equal, true);
}

TEST(single_test, mix_bin) {
    task expected_result[TASK_COUNT] = {
        {0, 0}, {1, 10000000},
        {2, 20000000}, {3, 30000000},
        {4, 40000000}, {5, 50000000},
        {6, 60000000}, {7, 70000000},
        {8, 80000000}, {9, 90000000}
    };

    task *received_result = single_thread_run(fds_buf[1]);

    bool are_equal = true;

    for (int i = 0; i < TASK_COUNT; i++) {
        are_equal = task_compare(&expected_result[i], &received_result[i]);

        if (!are_equal) {
            break;
        }
    }

    free(received_result);

    EXPECT_EQ(are_equal, true);
}


TEST(thread_test, mix_bin) {
    task expected_result[TASK_COUNT] = {
        {0, 0}, {1, 10000000},
        {2, 20000000}, {3, 30000000},
        {4, 40000000}, {5, 50000000},
        {6, 60000000}, {7, 70000000},
        {8, 80000000}, {9, 90000000}
    };

    task *received_result = multi_thread_run(fds_buf[1]);

    bool are_equal = true;

    for (int i = 0; i < TASK_COUNT; i++) {
        are_equal = task_compare(&expected_result[i], &received_result[i]);

        if (!are_equal) {
            break;
        }
    }

    free(received_result);

    EXPECT_EQ(are_equal, true);
}

TEST(dual_test, rand_bin) {
    task *received_result_single = single_thread_run(fds_buf[2]);
    task *received_result_thread = multi_thread_run(fds_buf[2]);

    bool are_equal = true;

    for (int i = 0; i < TASK_COUNT; i++) {
        are_equal = task_compare(&received_result_single[i], &received_result_thread[i]);

        if (!are_equal) {
            break;
        }
    }

    free(received_result_single);
    free(received_result_thread);

    EXPECT_EQ(are_equal, true);
}

TEST(single_stress_test, three_in_one) {
    struct timespec start = {};
    struct timespec finish = {};

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < 3; i++) {
        task *task_result = single_thread_run(fds_buf[i]);
        free(task_result);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);

    double single_thread_time = (finish.tv_sec - start.tv_sec);
    single_thread_time += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    std::cout << '\n' << '\n' << "Time result for single thread: " << single_thread_time << "s." << '\n' << '\n';
}

TEST(thread_stress_test, three_in_one) {
    struct timespec start = {};
    struct timespec finish = {};

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < 3; i++) {
        task *task_result = multi_thread_run(fds_buf[i]);
        free(task_result);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);

    double single_thread_time = (finish.tv_sec - start.tv_sec);
    single_thread_time += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


    int number_of_cores = sysconf(_SC_NPROCESSORS_ONLN);

    std::cout << '\n' << '\n' << "Time result for " << number_of_cores;
    std::cout << " threads: " << single_thread_time << " s." << '\n' << '\n';
}

TEST(close_test_file, closing) {
    int check = close_test_files(fds_buf);

    fclose(stdout);
    fclose(stderr);
    fclose(stdin);

    EXPECT_EQ(check, 0);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
