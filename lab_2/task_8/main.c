#include "functions.h"

int main()
{
    char *res;

    sum_any_base_all_numbers(&res, 16, 2, "FFFFFFFF", "FFFFFF");
    printf("%s\n", res);
    free(res);
}
