#include "input_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

void readLine(const char *prompt, char *buffer, size_t size)
{
    size_t len;
    int ch;

    if (size == 0) return;
    for (;;)
    {
        printf("%s", prompt);
        if (fgets(buffer, (int)size, stdin) == NULL)
        {
            buffer[0] = '\0';
            return;
        }
        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
            return;
        }
        if (len < size - 1)
            return;

        while ((ch = getchar()) != '\n' && ch != EOF) { }
        printf("Input is too long. Please try again.\n");
    }
}

int readInt(const char *prompt, int minValue, int maxValue)
{
    char line[128], *end;
    long value;

    for (;;)
    {
        readLine(prompt, line, sizeof line);
        errno = 0;
        value = strtol(line, &end, 10);
        while (isspace((unsigned char)*end)) end++;
        if (line[0] != '\0' && *end == '\0' && errno == 0 &&
            value >= minValue && value <= maxValue)
            return (int)value;
        printf("Please enter a whole number from %d to %d.\n", minValue, maxValue);
    }
}

float readFloat(const char *prompt, float minValue, float maxValue)
{
    char line[128], *end;
    float value;

    for (;;)
    {
        readLine(prompt, line, sizeof line);
        errno = 0;
        value = strtof(line, &end);
        while (isspace((unsigned char)*end)) end++;
        if (line[0] != '\0' && *end == '\0' && errno == 0 &&
            value >= minValue && value <= maxValue)
            return value;
        printf("Please enter a number from %.2f to %.2f.\n", minValue, maxValue);
    }
}

int isDigits(const char *text)
{
    size_t i;
    if (text == NULL || text[0] == '\0') return 0;
    for (i = 0; text[i] != '\0'; i++)
        if (!isdigit((unsigned char)text[i])) return 0;
    return 1;
}
