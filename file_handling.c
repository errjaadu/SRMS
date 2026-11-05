#include "file_handling.h"
#include "academic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "students.dat"
#define TEXT_FILE "students.txt"

int loadRecords(Student students[], int *count)
{
    FILE *fp;
    long fileSize;
    size_t recordsRead;

    *count = 0;
    fp = fopen(DATA_FILE, "rb");
    if (fp == NULL)
    {
        /* A missing file is normal on the first run. */
        return 1;
    }

    if (fseek(fp, 0, SEEK_END) != 0)
    {
        fclose(fp);
        printf("Error checking the data file.\n");
        return 0;
    }
    fileSize = ftell(fp);
    if (fileSize < 0 || fileSize % (long)sizeof(Student) != 0 ||
        fileSize / (long)sizeof(Student) > MAX_STUDENTS)
    {
        fclose(fp);
        printf("The data file appears to be invalid or incompatible. It was not changed.\n");
        return 0;
    }
    rewind(fp);

    recordsRead = fread(students, sizeof(Student),
                        (size_t)(fileSize / (long)sizeof(Student)), fp);
    if (ferror(fp) || recordsRead != (size_t)(fileSize / (long)sizeof(Student)))
    {
        fclose(fp);
        printf("Error reading student records.\n");
        return 0;
    }
    if (fclose(fp) != 0)
    {
        printf("Warning: could not close the data file cleanly.\n");
        return 0;
    }

    *count = (int)recordsRead;
    return 1;
}

int saveRecords(const Student students[], int count)
{
    FILE *fp;
    size_t written;

    if (count < 0 || count > MAX_STUDENTS) return 0;
    fp = fopen(DATA_FILE, "wb");
    if (fp == NULL)
    {
        printf("Unable to open %s for writing.\n", DATA_FILE);
        return 0;
    }

    written = fwrite(students, sizeof(Student), (size_t)count, fp);
    if (written != (size_t)count || fclose(fp) != 0)
    {
        printf("Error saving student records.\n");
        return 0;
    }
    return 1;
}

int exportToText(const Student students[], int count)
{
    FILE *fp;
    int i, j;

    fp = fopen(TEXT_FILE, "w");
    if (fp == NULL)
    {
        printf("Unable to create %s.\n", TEXT_FILE);
        return 0;
    }

    fprintf(fp, "STUDENT RECORD MANAGEMENT SYSTEM\n");
    fprintf(fp, "Total records: %d\n\n", count);

    for (i = 0; i < count; i++)
    {
        fprintf(fp, "========================================\n");
        fprintf(fp, "ID: %d\nName: %s\nAge: %d\nGender: %s\n",
                students[i].id, students[i].name, students[i].age, students[i].gender);
        fprintf(fp, "Course: %s\nDepartment: %s\nSemester: %d\nPhone: %s\n",
                students[i].course, students[i].department,
                students[i].semester, students[i].phone);
        fprintf(fp, "Subjects and marks:\n");
        for (j = 0; j < SUBJECTS; j++)
            fprintf(fp, "  %s: %.2f/100\n",
                    students[i].subjectNames[j], students[i].marks[j]);
        fprintf(fp, "Total: %.2f/500\nPercentage: %.2f%%\nGrade: %s\n",
                students[i].total, students[i].percentage, students[i].grade);
    }

    if (fclose(fp) != 0)
    {
        printf("Error closing the text export.\n");
        return 0;
    }
    return 1;
}
