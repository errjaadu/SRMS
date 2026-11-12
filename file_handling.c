#include "file_handling.h"
#include "academic.h"
#include "student_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define DATA_FILE "students.dat"
#define TEXT_FILE "students.txt"
#define CSV_FILE "students.csv"
#define CSV_LINE_SIZE 8192
#define CSV_FIELDS (8 + SUBJECTS * 2)

int loadRecords(Student students[], int *count)
{
    FILE *fp;
    long fileSize;
    size_t recordsRead;

    *count = 0;
    fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) return 1;

    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        printf("Error checking the data file.\n");
        return 0;
    }
    fileSize = ftell(fp);
    if (fileSize < 0 || fileSize % (long)sizeof(Student) != 0 ||
        fileSize / (long)sizeof(Student) > MAX_STUDENTS) {
        fclose(fp);
        printf("The data file appears to be invalid or incompatible. It was not changed.\n");
        return 0;
    }
    rewind(fp);
    recordsRead = fread(students, sizeof(Student),
                        (size_t)(fileSize / (long)sizeof(Student)), fp);
    if (ferror(fp) || recordsRead != (size_t)(fileSize / (long)sizeof(Student))) {
        fclose(fp);
        printf("Error reading student records.\n");
        return 0;
    }
    if (fclose(fp) != 0) {
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
    if (fp == NULL) {
        printf("Unable to open %s for writing.\n", DATA_FILE);
        return 0;
    }
    written = fwrite(students, sizeof(Student), (size_t)count, fp);
    if (written != (size_t)count) {
        fclose(fp);
        printf("Error saving student records.\n");
        return 0;
    }
    if (fclose(fp) != 0) {
        printf("Error closing the data file.\n");
        return 0;
    }
    return 1;
}

static void writeCsvField(FILE *fp, const char *value)
{
    const char *p;
    int quote = strpbrk(value, ",\"\r\n") != NULL;
    if (quote) fputc('"', fp);
    for (p = value; *p; ++p) {
        if (*p == '"') fputc('"', fp);
        fputc(*p, fp);
    }
    if (quote) fputc('"', fp);
}

int exportToCSV(const Student students[], int count, const char *filename)
{
    FILE *fp;
    int i, j;
    if (!filename || !*filename || count < 0 || count > MAX_STUDENTS) return 0;
    fp = fopen(filename, "w");
    if (!fp) {
        printf("Unable to create %s.\n", filename);
        return 0;
    }
    fprintf(fp, "id,name,age,gender,course,department,semester,phone");
    for (j = 0; j < SUBJECTS; ++j)
        fprintf(fp, ",subject%d,marks%d", j + 1, j + 1);
    fputc('\n', fp);

    for (i = 0; i < count; ++i) {
        const Student *s = &students[i];
        fprintf(fp, "%d,", s->id); writeCsvField(fp, s->name);
        fprintf(fp, ",%d,", s->age); writeCsvField(fp, s->gender);
        fputc(',', fp); writeCsvField(fp, s->course);
        fputc(',', fp); writeCsvField(fp, s->department);
        fprintf(fp, ",%d,", s->semester); writeCsvField(fp, s->phone);
        for (j = 0; j < SUBJECTS; ++j) {
            fprintf(fp, ","); writeCsvField(fp, s->subjectNames[j]);
            fprintf(fp, ",%.2f", s->marks[j]);
        }
        fputc('\n', fp);
    }
    if (fclose(fp) != 0) {
        printf("Error closing CSV export.\n");
        return 0;
    }
    return 1;
}

/* Existing readable text export is retained for compatibility. */
int exportToText(const Student students[], int count)
{
    FILE *fp;
    int i, j;
    fp = fopen(TEXT_FILE, "w");
    if (fp == NULL) {
        printf("Unable to create %s.\n", TEXT_FILE);
        return 0;
    }
    fprintf(fp, "STUDENT RECORD MANAGEMENT SYSTEM\n");
    fprintf(fp, "Total records: %d\n\n", count);
    for (i = 0; i < count; i++) {
        fprintf(fp, "========================================\n");
        fprintf(fp, "ID: %d\nName: %s\nAge: %d\nGender: %s\n",
                students[i].id, students[i].name, students[i].age, students[i].gender);
        fprintf(fp, "Course: %s\nDepartment: %s\nSemester: %d\nPhone: %s\n",
                students[i].course, students[i].department,
                students[i].semester, students[i].phone);
        fprintf(fp, "Subjects and marks:\n");
        for (j = 0; j < SUBJECTS; j++)
            fprintf(fp, "  %s: %.2f/100\n", students[i].subjectNames[j], students[i].marks[j]);
        fprintf(fp, "Total: %.2f/500\nPercentage: %.2f%%\nGrade: %s\n",
                students[i].total, students[i].percentage, students[i].grade);
    }
    if (fclose(fp) != 0) {
        printf("Error closing the text export.\n");
        return 0;
    }
    return 1;
}

static int parseCsvRow(char *line, char fields[][MAX_NAME + 64], int maxFields)
{
    int n = 0;
    char *p = line;
    while (*p) {
        size_t len = 0;
        int quoted = 0;
        if (n >= maxFields) return -1;
        if (*p == '"') { quoted = 1; ++p; }
        while (*p) {
            if (quoted) {
                if (*p == '"' && p[1] == '"') { 
                    if (len + 1 >= MAX_NAME + 64) return -1;
                    fields[n][len++] = '"'; p += 2; continue;
                }
                if (*p == '"') { ++p; quoted = 0; break; }
            } else if (*p == ',' || *p == '\n' || *p == '\r') break;
            if (len + 1 >= MAX_NAME + 64) return -1;
            fields[n][len++] = *p++;
        }
        if (quoted) return -1;
        fields[n][len] = '\0';
        if (*p == ',') { ++p; ++n; if (!*p) { if (n >= maxFields) return -1; fields[n++][0] = '\0'; } }
        else break;
    }
    return n + 1;
}

static int parseIntField(const char *s, int min, int max, int *out)
{
    char *end;
    long v;
    errno = 0;
    v = strtol(s, &end, 10);
    while (*end == ' ' || *end == '\t') ++end;
    if (errno || end == s || *end || v < min || v > max) return 0;
    *out = (int)v;
    return 1;
}

static int parseFloatField(const char *s, float *out)
{
    char *end;
    float v;
    errno = 0;
    v = strtof(s, &end);
    while (*end == ' ' || *end == '\t') ++end;
    if (errno || end == s || *end || v < 0.0f || v > 100.0f) return 0;
    *out = v;
    return 1;
}

static int validText(const char *s, size_t capacity)
{
    return s[0] != '\0' && strlen(s) < capacity;
}

int importFromCSV(Student students[], int *count, const char *filename)
{
    FILE *fp;
    char line[CSV_LINE_SIZE];
    char fields[CSV_FIELDS][MAX_NAME + 64];
    int lineNo = 0, added = 0, skipped = 0, invalid = 0;

    if (!filename || !*filename || *count < 0 || *count > MAX_STUDENTS) return 0;
    fp = fopen(filename, "r");
    if (!fp) {
        printf("Cannot open %s. Put the CSV file in the program folder or enter its full path.\n", filename);
        return 0;
    }
    if (!fgets(line, sizeof line, fp)) {
        fclose(fp);
        printf("The CSV file is empty.\n");
        return 0;
    }
    lineNo++;
    /* Accept our exported header; reject unrelated CSV files. */
    if (strncmp(line, "id,name,age,gender,course,department,semester,phone", 51) != 0) {
        fclose(fp);
        printf("Unrecognized CSV format. Use the CSV exported by this SRMS.\n");
        return 0;
    }

    while (fgets(line, sizeof line, fp)) {
        Student s;
        int n, j, id, age, semester;
        char department[MAX_DEPARTMENT];
        lineNo++;
        if (!strchr(line, '\n') && !feof(fp)) {
            int ch; while ((ch = fgetc(fp)) != '\n' && ch != EOF) {}
            invalid++; printf("Line %d: row is too long; skipped.\n", lineNo); continue;
        }
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') continue;
        n = parseCsvRow(line, fields, CSV_FIELDS);
        if (n != CSV_FIELDS) {
            invalid++; printf("Line %d: expected %d columns; skipped.\n", lineNo, CSV_FIELDS); continue;
        }
        if (!parseIntField(fields[0], 1, 2147483647, &id) ||
            !parseIntField(fields[2], 16, 60, &age) ||
            !parseIntField(fields[6], 1, MAX_SEMESTER, &semester) ||
            !validText(fields[1], MAX_NAME) ||
            !validText(fields[3], MAX_GENDER) ||
            !validText(fields[4], MAX_COURSE) ||
            !validText(fields[5], MAX_DEPARTMENT) ||
            !validText(fields[7], MAX_PHONE) || strlen(fields[7]) != 10) {
            invalid++; printf("Line %d: invalid student details; skipped.\n", lineNo); continue;
        }
        strcpy(department, fields[5]);
        for (j = 0; department[j]; ++j) department[j] = (char)toupper((unsigned char)department[j]);
        if (strcmp(department, "CSE") && strcmp(department, "ECE") &&
            strcmp(department, "ME") && strcmp(department, "CE") && strcmp(department, "EE")) {
            invalid++; printf("Line %d: invalid department; skipped.\n", lineNo); continue;
        }
        if (findStudentById(students, *count, id) != -1) {
            skipped++; printf("Line %d: ID %d already exists; skipped.\n", lineNo, id); continue;
        }
        if (*count >= MAX_STUDENTS) {
            printf("Student limit (%d) reached; remaining rows were not imported.\n", MAX_STUDENTS);
            break;
        }
        memset(&s, 0, sizeof s);
        s.id = id; s.age = age; s.semester = semester;
        strcpy(s.name, fields[1]); strcpy(s.gender, fields[3]);
        strcpy(s.course, fields[4]); strcpy(s.department, department);
        strcpy(s.phone, fields[7]);
        for (j = 0; j < SUBJECTS; ++j) {
            if (!validText(fields[8 + j * 2], MAX_SUBJECT_NAME) ||
                !parseFloatField(fields[9 + j * 2], &s.marks[j])) break;
            strcpy(s.subjectNames[j], fields[8 + j * 2]);
        }
        if (j != SUBJECTS) {
            invalid++; printf("Line %d: invalid subject or marks; skipped.\n", lineNo); continue;
        }
        calculateStudentResult(&s);
        students[(*count)++] = s;
        added++;
    }
    if (ferror(fp)) { fclose(fp); printf("Error reading CSV file.\n"); return 0; }
    fclose(fp);
    printf("\nImport summary: %d added, %d duplicate IDs skipped, %d invalid rows skipped.\n",
           added, skipped, invalid);
    return added;
}
