#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <preparation.h>
#include <pthread.h>

task *multi_thread_run(int fd);

#ifdef __cplusplus
}
#endif
