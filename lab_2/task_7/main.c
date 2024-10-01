#include "functions.h"

int main(int argc, char *argv[])
{
    double result;
    dichotomy_method(&result, 18., 30., 0.0000000000001, func5);
    printf("%lf\n", result);
}