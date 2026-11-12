#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include "student.h"

int loadRecords(Student students[], int *count);
int saveRecords(const Student students[], int count);
int exportToText(const Student students[], int count);
int exportToCSV(const Student students[], int count, const char *filename);
int importFromCSV(Student students[], int *count, const char *filename);

#endif
