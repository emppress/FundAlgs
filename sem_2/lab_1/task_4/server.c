#include "functions.h"

int main()
{
    int msg_id;
    state_t state;
    my_msg_buf msg;
    status st = SUCCESS;
    key_t key = ftok("server", 65);
    if (key == -1)
    {
        printf("Key error\n");
        return IPC_ERROR;
    }

    state.boat_full = state.cabbage_in_boat = state.goat_in_boat = state.wolf_in_boat = 0;
    state.boat_side = state.cabbage_side = state.goat_side = state.wolf_side = left;

    msg_id = msgget(key, IPC_CREAT | 0666);
    if (msg_id == -1)
    {
        printf("Msgget error\n");
        return IPC_ERROR;
    }

    do
    {
        if (msgrcv(msg_id, &msg, sizeof(my_msg_buf) - sizeof(long), 1, 0) == -1)
        {
            st = IPC_ERROR;
            break;
        }

        st = proccess_command(&state, msg.m_text, msg_id, msg.id);
        if (st == GAME_OVER)
        {
            printf("The game is over, DEFEAT! :(\n");
        }
        else if (st == VICTORY)
        {
            printf("The game is over, VICTORY! :)\n");
        }
    } while (st != IPC_ERROR && st != GAME_OVER && st != VICTORY);

    if (msgctl(msg_id, IPC_RMID, NULL) == -1 || st == IPC_ERROR)
    {
        printf("Ipc error\n");
        return IPC_ERROR;
    }

    return SUCCESS;
}