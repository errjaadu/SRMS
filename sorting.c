#include "sorting.h"
#include "student_ops.h"
#include "input_utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void swapStudents(Student *a, Student *b)
{
    Student temp = *a;
    *a = *b;
    *b = temp;
}

void sortByID(Student students[], int count)
{
    int i, j;
    for (i = 0; i < count - 1; i++)
        for (j = 0; j < count - i - 1; j++)
            if (students[j].id > students[j + 1].id)
                swapStudents(&students[j], &students[j + 1]);
}

void sortByName(Student students[], int count)
{
    int i, j;
    for (i = 0; i < count - 1; i++)
        for (j = 0; j < count - i - 1; j++)
            if (strcmp(students[j].name, students[j + 1].name) > 0)
                swapStudents(&students[j], &students[j + 1]);
}

void sortByPercentage(Student students[], int count)
{
    int i, j;
    for (i = 0; i < count - 1; i++)
        for (j = 0; j < count - i - 1; j++)
            if (students[j].percentage < students[j + 1].percentage)
                swapStudents(&students[j], &students[j + 1]);
}

void sortByTotal(Student students[], int count)
{
    int i, j;
    for (i = 0; i < count - 1; i++)
        for (j = 0; j < count - i - 1; j++)
            if (students[j].total < students[j + 1].total)
                swapStudents(&students[j], &students[j + 1]);
}

static void printFilterHeader(void)
{
    printf("\n========== FILTERED STUDENTS ==========\n");
}

void filterStudents(const Student students[], int count)
{
    int choice, semester, i, found = 0;
    char department[MAX_DEPARTMENT];
    const char *allowed[] = {"CSE", "ECE", "ME", "CE", "EE"};

    printf("\n1. Department\n2. Semester\n3. Department and semester\n");
    choice = readInt("Choose filter: ", 1, 3);

    if (choice == 1 || choice == 3)
    {
        int valid = 0, k;
        do
        {
            readLine("Department (CSE/ECE/ME/CE/EE): ", department, sizeof department);
            for (k = 0; department[k]; k++) department[k] = (char)toupper((unsigned char)department[k]);
            for (k = 0; k < 5; k++)
                if (strcmp(department, allowed[k]) == 0) valid = 1;
            if (!valid) printf("Invalid department.\n");
        } while (!valid);
    }
    if (choice == 2 || choice == 3)
        semester = readInt("Semester (1-8): ", 1, MAX_SEMESTER);

    printFilterHeader();
    for (i = 0; i < count; i++)
    {
        int match = 1;
        if ((choice == 1 || choice == 3) &&
            strcmp(students[i].department, department) != 0) match = 0;
        if ((choice == 2 || choice == 3) &&
            students[i].semester != semester) match = 0;
        if (match)
        {
            displaySingleStudent(&students[i]);
            found++;
        }
    }
    if (!found) printf("No matching records found.\n");
}
