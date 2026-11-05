#ifndef ACADEMIC_H
#define ACADEMIC_H

#include "student.h"

float calculateTotal(const float marks[], int subjects);
float calculatePercentage(float total, int subjects);
void calculateGrade(float percentage, char grade[]);
void calculateStudentResult(Student *student);
void displayResult(const Student *student);

#endif
