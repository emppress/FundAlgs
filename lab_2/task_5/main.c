#include "functions.h"

int main(int argc, char *argv[])
{
    char buff[BUFSIZ];

    printf("\t\tTest 1\n");
    oversprintf(buff, "%d %d %Ro %.2f", 3888, 3, 4, 3.22);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 2\n");
    oversprintf(buff, "strokaaaaa %Ro", 6);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 3\n");
    oversprintf(buff, "strokaaaaa %Roover", 567);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 4\n");
    oversprintf(buff, "strokaaaaa %Roover", 100000000);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 5\n");
    oversprintf(buff, "strokaaaaa pupupu %Ro", 5);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 6\n");
    oversprintf(buff, "strokaaaaa %", 100000000);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 7\n");
    oversprintf(buff, "strokaaaaa %Cv", 123, 2);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 8\n");
    oversprintf(buff, "strokaaaaa %Cv", -123, 16);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 9\n");
    oversprintf(buff, "strokaaaaa %CV", 123, 2);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 10\n");
    oversprintf(buff, "strokaaaaa %CV", -123, 16);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 11\n");
    oversprintf(buff, "strokaaaaa %to", "1111011", 2);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 12\n");
    oversprintf(buff, "strokaaaaa %to", "-7b", 16);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 13\n");
    oversprintf(buff, "strokaaaaa %TO", "-7B", 10);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 14\n");
    oversprintf(buff, "%mi", -123);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 15\n");
    oversprintf(buff, "%mu", 123);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 16\n");
    oversprintf(buff, "%md", 0.1);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 17\n");
    oversprintf(buff, "%mf", (float)10034.34);
    printf("string: '%s'\n\n", buff);

    printf("\t\tTest 18\n");
    oversprintf(buff, "%Zr", 10);
    printf("string: '%s'\n\n", buff);

    FILE *output = fopen("test.txt", "w");
    if (output == NULL)
        return FILE_OPEN_ERROR;

    overfprintf(output, "%d %d %Ro %.2f\n", 3888, 3, 4, 3.22);

    overfprintf(output, "rororoRo%% %Ro\n", 6);

    overfprintf(output, "ss %Roodddr\n", 567);

    overfprintf(output, " %Roooon\n", 100000000);

    overfprintf(output, "%Ro %.2f bubedi\n", 5, 234567.89);

    overfprintf(output, " %\n", 100000000);

    overfprintf(output, " %Cv\n", 123, 2);

    overfprintf(output, "%Cv\n", -123, 16);

    overfprintf(output, "%CV\n", 123, 2);

    overfprintf(output, "strokaaaaa %CV\n", -123, 16);

    overfprintf(output, "aboba %to\n", "1111011", 2);

    overfprintf(output, "rrr %to\n", "-7b", 16);

    overfprintf(output, "to%TOto\n", "-7B", 10);

    overfprintf(output, "%mi\n", -123);

    overfprintf(output, "%mu\n", 123);

    overfprintf(output, "%md\n", 0.1);

    overfprintf(output, "%mf\n", -1.2);

    overfprintf(output, "%Zr\n", 10);

    fclose(output);
    return 0;
}