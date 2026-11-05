#include "student_ops.h"
#include "input_utils.h"
#include "academic.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void readRequiredText(const char *prompt, char *buffer, size_t size)
{
    do
    {
        readLine(prompt, buffer, size);
        if (buffer[0] == '\0') printf("This field cannot be empty.\n");
    } while (buffer[0] == '\0');
}

static void readPhone(char phone[MAX_PHONE])
{
    do
    {
        readLine("Phone (10 digits): ", phone, MAX_PHONE);
        if (strlen(phone) != 10 || !isDigits(phone))
            printf("Enter exactly 10 digits.\n");
    } while (strlen(phone) != 10 || !isDigits(phone));
}

static void readDepartment(char department[MAX_DEPARTMENT])
{
    const char *departments[] = {"CSE", "ECE", "ME", "CE", "EE"};
    char value[MAX_DEPARTMENT];
    int i, valid;

    do
    {
        readRequiredText("Department (CSE/ECE/ME/CE/EE): ", value, sizeof value);
        for (i = 0; value[i]; i++) value[i] = (char)toupper((unsigned char)value[i]);
        valid = 0;
        for (i = 0; i < 5; i++)
            if (strcmp(value, departments[i]) == 0) valid = 1;
        if (!valid) printf("Choose one of the listed departments.\n");
    } while (!valid);
    strcpy(department, value);
}

static void readStudentDetails(Student *s, int includeId)
{
    int i;
    if (includeId)
        s->id = readInt("Student ID (positive integer): ", 1, 2147483647);

    readRequiredText("Full name: ", s->name, sizeof s->name);
    s->age = readInt("Age (16-60): ", 16, 60);
    readRequiredText("Gender: ", s->gender, sizeof s->gender);
    readRequiredText("Course: ", s->course, sizeof s->course);
    readDepartment(s->department);
    s->semester = readInt("Semester (1-8): ", 1, MAX_SEMESTER);
    readPhone(s->phone);

    printf("\nEnter the five subject names and marks:\n");
    for (i = 0; i < SUBJECTS; i++)
    {
        char prompt[80];
        snprintf(prompt, sizeof prompt, "Subject %d name: ", i + 1);
        readRequiredText(prompt, s->subjectNames[i], sizeof s->subjectNames[i]);
        snprintf(prompt, sizeof prompt, "Marks for %s (0-100): ", s->subjectNames[i]);
        s->marks[i] = readFloat(prompt, 0.0f, 100.0f);
    }
    calculateStudentResult(s);
}

int findStudentById(const Student students[], int count, int id)
{
    int i;
    for (i = 0; i < count; i++)
        if (students[i].id == id) return i;
    return -1;
}

void displaySingleStudent(const Student *s)
{
    int i;
    printf("\n----------------------------------------\n");
    printf("Student ID : %d\nName       : %s\nAge        : %d\nGender     : %s\n",
           s->id, s->name, s->age, s->gender);
    printf("Course     : %s\nDepartment : %s\nSemester   : %d\nPhone      : %s\n",
           s->course, s->department, s->semester, s->phone);
    for (i = 0; i < SUBJECTS; i++)
        printf("%-24s : %.2f\n", s->subjectNames[i], s->marks[i]);
    printf("Total      : %.2f / 500\nPercentage : %.2f%%\nGrade      : %s\n",
           s->total, s->percentage, s->grade);
    printf("Status     : %s\n", s->percentage >= 50.0f ? "PASS" : "FAIL");
    printf("----------------------------------------\n");
}

void displayStudents(const Student students[], int count)
{
    int i;
    if (count == 0)
    {
        printf("\nNo student records are available.\n");
        return;
    }
    printf("\n========== ALL STUDENT RECORDS (%d) ==========\n", count);
    for (i = 0; i < count; i++) displaySingleStudent(&students[i]);
}

int addStudent(Student students[], int *count)
{
    Student s;
    if (*count >= MAX_STUDENTS)
    {
        printf("Student capacity (%d) has been reached.\n", MAX_STUDENTS);
        return 0;
    }

    for (;;)
    {
        s.id = readInt("Student ID (positive integer): ", 1, 2147483647);
        if (findStudentById(students, *count, s.id) == -1) break;
        printf("That ID already exists. Enter a unique ID.\n");
    }
    readStudentDetails(&s, 0);
    students[*count] = s;
    (*count)++;
    printf("Student record added successfully.\n");
    return 1;
}

int updateStudent(Student students[], int count, int id)
{
    int index = findStudentById(students, count, id);
    Student updated;
    if (index == -1)
    {
        printf("Student ID %d was not found.\n", id);
        return 0;
    }

    updated = students[index];
    printf("\nEnter replacement details. The student ID will remain unchanged.\n");
    readStudentDetails(&updated, 0);
    students[index] = updated;
    printf("Student record updated successfully.\n");
    return 1;
}

int deleteStudent(Student students[], int *count, int id)
{
    int index = findStudentById(students, *count, id);
    int i;
    char answer[10];

    if (index == -1)
    {
        printf("Student ID %d was not found.\n", id);
        return 0;
    }

    printf("Delete record for %s (ID %d)? (y/n): ", students[index].name, id);
    readLine("", answer, sizeof answer);
    if (answer[0] != 'y' && answer[0] != 'Y')
    {
        printf("Deletion cancelled.\n");
        return 0;
    }

    for (i = index; i < *count - 1; i++) students[i] = students[i + 1];
    (*count)--;
    printf("Student record deleted.\n");
    return 1;
}

void viewStudentResult(const Student students[], int count, int id)
{
    int index = findStudentById(students, count, id);
    if (index == -1)
    {
        printf("Student ID %d was not found.\n", id);
        return;
    }
    displayResult(&students[index]);
}
