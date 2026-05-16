#ifndef ERRORS_H
#define ERRORS_H

typedef enum {
    ERROR_NONE = 0,
    ERROR_MUTEX_CREATE_FAILED,
    ERROR_MUTEX_TAKE_FAILED,
    ERROR_MUTEX_GIVE_FAILED,
} Error_t;

#endif
