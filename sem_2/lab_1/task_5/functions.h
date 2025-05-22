#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <inttypes.h>

#define COUNT_PEOPLE 15ul
#define SEM_KEY 1233

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    FILE_OPEN_ERROR,
    NOT_FOUND,
    FORK_ERROR,
    IPC_ERROR,
    RW_ERROR,
    PTHREAD_ERROR
} status;

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
union semun
{
    int val;               /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT, IPC_SET */
    unsigned short *array; /* array for GETALL, SETALL */
                           /* Linux specific part: */
    struct seminfo *__buf; /* buffer for IPC_INFO */
};
#endif

typedef enum State
{
    MAN = 1,
    WOMAN = 2,
    EMPTY = 0
} State;

#define STATE_TO_STRING(STATE) ((STATE) == EMPTY ? "EMPTY" : (STATE) == MAN ? "MAN" \
                                                                            : "WOMAN")

typedef struct BathArg
{
    size_t max_count;     // максимум людей в ванной
    size_t current_count; // текущее кол-во людей
    State state;          // состояние коматы
    int sem_id;           // id семафора
} BathArg;

status sem_init(int id, int val);
status string_to_size_t(const char *str, size_t *result);
void *bathroom_block(void *args);

#endif