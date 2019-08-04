#ifndef COMMON_LOG_H_
#define COMMON_LOG_H_

#include <stdio.h>
#include <stdlib.h>

#define LOG(...)                      \
    {                                 \
        fprintf(stdout, __VA_ARGS__); \
        fflush(stdout);               \
    }

#define LOG_FATAL(...)                \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
        fflush(stderr);               \
        exit(1);                      \
    }

#endif  // COMMON_LOG_H_