#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include "student.h"

int loadRecords(Student students[], int *count);
int saveRecords(const Student students[], int count);
int exportToText(const Student students[], int count);

#endif
