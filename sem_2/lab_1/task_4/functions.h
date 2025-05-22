#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <inttypes.h>
#include <string.h>

#define MSG_KEY 1234
#define MAX_COMMAND_SIZE 256

typedef struct my_msg_buf
{
    long mtype;
    long id;
    char m_text[MAX_COMMAND_SIZE];
} my_msg_buf;

typedef enum side
{
    left,
    right
} side;

typedef struct state_t
{
    char wolf_in_boat;
    char goat_in_boat;
    char cabbage_in_boat;
    char boat_full;
    side wolf_side;
    side goat_side;
    side cabbage_side;
    side boat_side;
} state_t;

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
    PTHREAD_ERROR,
    GAME_OVER,
    VICTORY
} status;

status proccess_command(state_t *state, const char *command, int msgid, long client_id);

#endif