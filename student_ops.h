#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include "student.h"

int addStudent(Student students[], int *count);
void displayStudents(const Student students[], int count);
int findStudentById(const Student students[], int count, int id);
void displaySingleStudent(const Student *student);
int updateStudent(Student students[], int count, int id);
int deleteStudent(Student students[], int *count, int id);
void viewStudentResult(const Student students[], int count, int id);

#endif
