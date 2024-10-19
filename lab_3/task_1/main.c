#include "functions.h"

int main()
{
    char ans[128];
    int size = 0, i;
    if (base10_to_basex(-11231231, 5, ans, &size))
    {
        printf("Input error\n");
        return INPUT_ERROR;
    }

    print_result(ans, size);

    return 0;
}