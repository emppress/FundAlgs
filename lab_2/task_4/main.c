#include "functions.h"

int main()
{
    puts("\nTest 4.1\n");
    int res;
    point p1 = (point){0, 0};
    point p2 = (point){1, 0};
    point p3 = (point){1, 1};
    point p4 = (point){0, 1};
    printf("\ttest 1\n{{0,0}, {1, 0}, {1, 1}, {0, 1}}\nexpected result: yes\n");
    res = is_convex(4, p1, p2, p3, p4);
    if (res)
    {
        printf("\t result: yes\n");
    }
    else
    {
        printf("\t result: no\n");
    }

    p1 = (point){0, 0};
    p2 = (point){1, 0};
    p3 = (point){0, 1};
    p4 = (point){1, 1};
    printf("\ttest 2\n{{0,0}, {1, 0}, {0, 1}, {1, 1}}\nexpected result: no\n");
    res = is_convex(4, p1, p2, p3, p4);
    if (res)
    {
        printf("\t result: yes\n");
    }
    else
    {
        printf("\t result: no\n");
    }

    // Тест 3: многоугольник с тремя вершинами
    p1 = (point){0, 0};
    p2 = (point){1, 0};
    p3 = (point){0, 1};
    printf("\ttest 3\n{{0,0}, {1, 0}, {0, 1}}\nexpected result: yes\n");
    res = is_convex(3, p1, p2, p3);
    if (res)
    {
        printf("\t result: yes\n");
    }
    else
    {
        printf("\t result: no\n");
    }

    // Тест 4: многоугольник с двумя вершинами
    p1 = (point){0, 0};
    p2 = (point){1, 0};
    printf("\ttest 4\n{{0,0}, {1, 0}}\nexpected result: yes\n");
    res = is_convex(2, p1, p2);
    if (res)
    {
        printf("\t result: yes\n");
    }
    else
    {
        printf("\t result: no\n");
    }

    // Тест 5: многоугольник с одной вершиной
    p1 = (point){0, 0};
    printf("\ttest 5\n{{0,0}}\nexpected result: yes\n");
    res = is_convex(1, p1);
    if (res)
    {
        printf("\t result: yes\n");
    }
    else
    {
        printf("\t result: no\n");
    }

    puts("\nTest 4.2\n");

    double result;
    // Test 1
    printf("\ttest 1\n2x^2 + 3x + 4 at x = 1\nexpected result: 9\n");
    switch (find_polynom(&result, 1.0, 2, 2.0, 3.0, 4.0))
    {
    case INPUT_ERROR:
        printf("Input error\n");
        return INPUT_ERROR;
        break;
    case OVERFLOW_ERROR:
        printf("Overflow error\n");
        return OVERFLOW_ERROR;
        break;

    case SUCCESS:
        printf("\t result: %f\n", result);
        break;
    }

    // Test 2
    printf("\n\ttest 2\n1x^3 + 2x^2 + 3x + 4 at x = 2\nexpected result: 26\n");
    switch (find_polynom(&result, 2.0, 3, 1.0, 2.0, 3.0, 4.0))
    {
    case INPUT_ERROR:
        printf("Input error\n");
        return INPUT_ERROR;
        break;
    case OVERFLOW_ERROR:
        printf("Overflow error\n");
        return OVERFLOW_ERROR;
        break;

    case SUCCESS:
        printf("\t result: %f\n", result);
        break;
    }

    // Test 3
    printf("\n\ttest 3\n1x^4 + 2x^3 + 3x^2 + 4x + 5 at x = 2\nexpected result: 57\n");
    switch (find_polynom(&result, 2.0, 4, 1.0, 2.0, 3.0, 4.0, 5.0))
    {
    case INPUT_ERROR:
        printf("Input error\n");
        return INPUT_ERROR;
        break;
    case OVERFLOW_ERROR:
        printf("Overflow error\n");
        return OVERFLOW_ERROR;
        break;

    case SUCCESS:
        printf("\t result: %f\n", result);
        break;
    }

    // Test 4
    printf("\n\ttest 4\n1x^5 + 2x^4 + 3x^3 + 4x^2 + 5x + 6 at x = 1\nexpected result: 21\n");
    switch (find_polynom(&result, 1.0, 5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0))
    {
    case INPUT_ERROR:
        printf("Input error\n");
        return INPUT_ERROR;
        break;
    case OVERFLOW_ERROR:
        printf("Overflow error\n");
        return OVERFLOW_ERROR;
        break;

    case SUCCESS:
        printf("\t result: %f\n", result);
        break;
    }

    printf("\n\ttest 5\nOverflow check\nexpected result: error\n");
    switch (find_polynom(&result, 1e20, 2, 1e300, 1e100, 1e307))
    {
    case INPUT_ERROR:
        printf("Input error\n");
        return INPUT_ERROR;
        break;
    case OVERFLOW_ERROR:
        printf("Overflow error\n");
        // return OVERFLOW_ERROR;
        break;

    case SUCCESS:
        printf("\t result: %f\n", result);
        break;
    }

    puts("\nTest 4.3\n");
    char ans[100];
    int count = 5;
    printf("\n\ttest 1\n16CC 1 AB 4ED 7E0 FA4\nexpected result: YES YES YES YES NO\n");
    if (is_kaprekar(count, ans, 16, "1", "AB", "4ED", "7E0", "FA4") != SUCCESS)
    {
        printf("Input error\n");
        return INPUT_ERROR;
    }
    else
        printf("\t result:");
    {
        for (int i = 0; i < count; ++i)
        {
            if (ans[i] == '+')
                printf(" YES");
            else
                printf(" NO");
        }
    }

    count = 4;
    printf("\n\ttest 2\n10CC 1 33 55 17344\nexpected result: YES NO YES YES \n");
    if (is_kaprekar(count, ans, 10, "1", "33", "55", "17344") != SUCCESS)
    {
        printf("Input error\n");
        return INPUT_ERROR;
    }
    else
        printf("\t result:");
    {
        for (int i = 0; i < count; ++i)
        {
            if (ans[i] == '+')
                printf(" YES");
            else
                printf(" NO");
        }
    }

    count = 6;
    printf("\n\ttest 3\n12CC 1 B 66 444 BB 10000\nexpected result: YES YES YES YES YES NO\n");
    if (is_kaprekar(count, ans, 12, "1", "B", "66", "444", "BB", "10000") != SUCCESS)
    {
        printf("Input error\n");
        return INPUT_ERROR;
    }
    else
        printf("\t result:");
    {
        for (int i = 0; i < count; ++i)
        {
            if (ans[i] == '+')
                printf(" YES");
            else
                printf(" NO");
        }
    }

    count = 2;
    printf("\n\ttest 4\nError check\nexpected result: Input error\n");
    if (is_kaprekar(count, ans, 10, "FF", "1") != SUCCESS)
    {
        printf("\t result: Input error\n");
        return INPUT_ERROR;
    }
    else
        printf("\t result:");
    {
        for (int i = 0; i < count; ++i)
        {
            if (ans[i] == '+')
                printf(" YES");
            else
                printf(" NO");
        }
    }

    return 0;
}
