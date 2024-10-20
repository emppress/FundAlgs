#ifndef _STATUS_H_
#define _STATUS_H_

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    FILE_OPEN_ERROR,
    MISSING
} status;

#endif