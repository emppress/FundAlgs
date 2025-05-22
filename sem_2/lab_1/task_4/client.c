#include "functions.h"

int main(int argc, char *argv[])
{
    long pid = (long)getpid();
    key_t key = ftok("server", 65);
    int msgid;

    if (argc != 2)
    {
        printf("Usage: %s <command file>\n", argv[0]);
        return INPUT_ERROR;
    }

    msgid = msgget(key, 0666);
    if (msgid == -1)
    {
        printf("Msgget error\n");
        return IPC_ERROR;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("Fopen error\n");
        return FILE_OPEN_ERROR;
    }

    my_msg_buf msg;
    msg.id = pid;
    msg.mtype = 1; // Тип сообщения для команд

    while (fgets(msg.m_text, sizeof(msg.m_text), file))
    {
        msg.id = pid;
        msg.mtype = 1;

        msg.m_text[strcspn(msg.m_text, ";")] = 0;
        printf("Read: %s\n", msg.m_text);

        if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0))
        {
            printf("Msgsnd error\n");
            fclose(file);
            return IPC_ERROR;
        }

        if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), pid, 0) == -1)
        {
            printf("Msgrcv error\n");
            fclose(file);
            return IPC_ERROR;
        }
        printf("Server response: %s\n", msg.m_text);
        if (!strncmp(msg.m_text, "Error", 5))
        {
            printf("Game over :(\n");
            break;
        }
        if (!strncmp(msg.m_text, "Victory", 7))
        {
            break;
        }
    }

    fclose(file);
    return 0;
}