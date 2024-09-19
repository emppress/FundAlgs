#include "functions.h"

void exclude_nums(FILE *in, FILE *out)
{
    int c;
    while ((c = fgetc(in)) != EOF)
    {
        if (c >= '0' && c <= '9')
            continue;
        fputc(c, out);
    }
}

void count_letters(FILE *in, FILE *out)
{
    int c, count_letters = 0, num_string = 1;

    while ((c = fgetc(in)) != EOF)
    {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            count_letters++;
        }
        else if (c == '\n')
        {
            fprintf(out, "%d) %d;\n", num_string, count_letters);
            count_letters = 0;
            num_string++;
        }
    }
    if (count_letters)
        fprintf(out, "%d) %d букв;\n", num_string, count_letters);
}

void count_symbols(FILE *in, FILE *out)
{
    int c, count_letters = 0, num_string = 1;

    while ((c = fgetc(in)) != EOF)
    {
        if (c == '\n')
        {
            fprintf(out, "%d) %d;\n", num_string, count_letters + 1);
            count_letters = 0;
            num_string++;
        }
        else if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' '))
        {
            count_letters++;
        }
    }
    if (count_letters)
        fprintf(out, "%d) %d;\n", num_string, count_letters);
}

void replace_symbols(FILE *in, FILE *out)
{
    int c;
    while ((c = fgetc(in)) != EOF)
    {
        if (isdigit(c))
            fputc(c, out);
        else
            fprintf(out, "%X", c);
    }
}