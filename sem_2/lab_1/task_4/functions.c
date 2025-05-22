#include "functions.h"

status send_response(int msgid, long client_id, const char *response)
{
    my_msg_buf msg;

    if (!response)
        return MEMORY_ERROR;

    msg.mtype = client_id;
    msg.id = 0;
    strcpy(msg.m_text, response);

    if (msgsnd(msgid, &msg, sizeof(my_msg_buf) - sizeof(long), 0) == -1)
        return IPC_ERROR;

    return SUCCESS;
}

status proccess_command(state_t *state, const char *command, int msgid, long client_id)
{
    char object[MAX_COMMAND_SIZE];

    if (!state || !command)
        return MEMORY_ERROR;

    if (!strncmp(command, "take", 4))
    {
        sscanf(command, "take %s", object);
        if (state->boat_full)
        {
            send_response(msgid, client_id, "Error: Boat is already full.");
            return GAME_OVER;
        }
        if (!strcmp(object, "wolf"))
        {
            if (state->boat_side != state->wolf_side)
            {
                send_response(msgid, client_id, "Error: Wolf and boat on different shores.");
                return GAME_OVER;
            }
            state->wolf_in_boat = 1;
            state->boat_full = 1;
            if (send_response(msgid, client_id, "Success: take."))
                return IPC_ERROR;
        }
        else if (!strcmp(object, "goat"))
        {
            if (state->boat_side != state->goat_side)
            {
                send_response(msgid, client_id, "Error: Goat and boat on different shores.");
                return GAME_OVER;
            }

            state->goat_in_boat = 1;
            state->boat_full = 1;
            if (send_response(msgid, client_id, "Success: take."))
                return IPC_ERROR;
        }
        else if (!strcmp(object, "cabbage"))
        {
            if (state->boat_side != state->cabbage_side)
            {
                send_response(msgid, client_id, "Error: Cabbage and boat on different shores.");
                return GAME_OVER;
            }
            state->cabbage_in_boat = 1;
            state->boat_full = 1;
            if (send_response(msgid, client_id, "Success: take."))
                return IPC_ERROR;
        }
        else
        {
            if (send_response(msgid, client_id, "Error: Unknown object."))
                return IPC_ERROR;
        }
    }

    else if (!strcmp(command, "put"))
    {
        if (!state->boat_full)
        {
            send_response(msgid, client_id, "Error: Boat is empty.");
            return GAME_OVER;
        }
        if (state->cabbage_in_boat)
        {
            state->cabbage_side = state->boat_side;
            state->boat_full = state->cabbage_in_boat = 0;
        }
        else if (state->wolf_in_boat)
        {
            state->wolf_side = state->boat_side;
            state->boat_full = state->wolf_in_boat = 0;
        }
        else
        {
            state->goat_side = state->boat_side;
            state->boat_full = state->goat_in_boat = 0;
        }

        if (state->cabbage_side == state->wolf_side && state->cabbage_side == state->goat_side && state->goat_side == right)
        {
            if (send_response(msgid, client_id, "Victory!"))
                return IPC_ERROR;

            return VICTORY;
        }
        if (send_response(msgid, client_id, "Success: put."))
            return IPC_ERROR;
    }
    else if (!strcmp(command, "move"))
    {
        state->boat_side = !state->boat_side;
        if (state->wolf_side == state->goat_side && !state->wolf_in_boat && !state->goat_in_boat)
        {
            send_response(msgid, client_id, "Error: Wolf will eat the goat!");
            return GAME_OVER;
        }
        if (state->cabbage_side == state->goat_side && !state->goat_in_boat && !state->cabbage_in_boat)
        {
            send_response(msgid, client_id, "Error: Goat will eat the cabbage!");
            return GAME_OVER;
        }
        if (send_response(msgid, client_id, "Success: move."))
            return IPC_ERROR;
    }
    else
    {
        if (send_response(msgid, client_id, "Error: Unknown command."))
            return IPC_ERROR;
    }
    return SUCCESS;
}