#include "functions.h"

status string_to_ll(const char *str, long long *result, int base)
{
    errno = 0;
    char *endptr;
    *result = strtol(str, &endptr, base);
    if (errno == ERANGE && (*result == LLONG_MAX || *result == LLONG_MIN))
        return INPUT_ERROR;
    if (errno != 0 && *result == 0 || *endptr != '\0')
        return INPUT_ERROR;
    return OK;
}

status convert_to_xbase(long long num, int base, char *result)
{
}