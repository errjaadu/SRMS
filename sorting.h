#ifndef SORTING_H
#define SORTING_H

#include "student.h"

void sortByID(Student students[], int count);
void sortByName(Student students[], int count);
void sortByPercentage(Student students[], int count);
void sortByTotal(Student students[], int count);
void filterStudents(const Student students[], int count);

#endif
