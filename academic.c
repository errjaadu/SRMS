#include "academic.h"
#include <stdio.h>
#include <string.h>

float calculateTotal(const float marks[], int subjects)
{
    int i;
    float total = 0.0f;
    for (i = 0; i < subjects; i++) total += marks[i];
    return total;
}

float calculatePercentage(float total, int subjects)
{
    if (subjects <= 0) return 0.0f;
    return total / ((float)subjects * 100.0f) * 100.0f;
}

void calculateGrade(float percentage, char grade[])
{
    if (percentage >= 90.0f) strcpy(grade, "α+");
    else if (percentage >= 80.0f) strcpy(grade, "α");
    else if (percentage >= 70.0f) strcpy(grade, "α−");
    else if (percentage >= 60.0f) strcpy(grade, "β");
    else if (percentage >= 50.0f) strcpy(grade, "γ");
    else strcpy(grade, "F");
}

void calculateStudentResult(Student *student)
{
    student->total = calculateTotal(student->marks, SUBJECTS);
    student->percentage = calculatePercentage(student->total, SUBJECTS);
    calculateGrade(student->percentage, student->grade);
}

void displayResult(const Student *student)
{
    int i;
    printf("\n--------------- ACADEMIC RESULT ---------------\n");
    printf("Student ID : %d\nName       : %s\n", student->id, student->name);
    for (i = 0; i < SUBJECTS; i++)
        printf("%-25s : %6.2f / 100\n", student->subjectNames[i], student->marks[i]);
    printf("-----------------------------------------------\n");
    printf("Total      : %.2f / 500\n", student->total);
    printf("Percentage : %.2f%%\n", student->percentage);
    printf("Grade      : %s\n", student->grade);
    printf("Status     : %s\n", student->percentage >= 50.0f ? "PASS" : "FAIL");
    printf("-----------------------------------------------\n");
}
