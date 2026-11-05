#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include <stddef.h>

void readLine(const char *prompt, char *buffer, size_t size);
int readInt(const char *prompt, int minValue, int maxValue);
float readFloat(const char *prompt, float minValue, float maxValue);
int isDigits(const char *text);

#endif
