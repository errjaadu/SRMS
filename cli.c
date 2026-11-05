#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cli.h"
#include "auth.h"
#include "student.h"
#include "student_ops.h"
#include "file_handling.h"
#include "academic.h"
#include "sorting.h"
#include "statistics.h"

#define VERSION "2.0"

static int parsePositiveInt(const char *value, int *result)
{
    char *end;
    long number;

    errno = 0;
    number = strtol(value, &end, 10);

    if (errno != 0 || end == value || *end != '\0' ||
        number <= 0 || number > 2147483647L)
        return 0;

    *result = (int)number;
    return 1;
}

static int saveChanges(Student students[], int count)
{
    if (!saveRecords(students, count))
    {
        printf("Error: Could not save student records.\n");
        return 0;
    }

    if (!exportToText(students, count))
    {
        printf("Warning: Records saved, but text export failed.\n");
        return 0;
    }

    printf("Changes saved successfully.\n");
    return 1;
}

static void filterByDepartment(
    const Student students[], int count, const char *department)
{
    int found = 0;

    printf("\nStudents in department: %s\n", department);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(students[i].department, department) == 0)
        {
            displaySingleStudent(&students[i]);
            found++;
        }
    }

    if (!found)
        printf("No matching students found.\n");
}

static void filterBySemester(
    const Student students[], int count, int semester)
{
    int found = 0;

    printf("\nStudents in semester: %d\n", semester);

    for (int i = 0; i < count; i++)
    {
        if (students[i].semester == semester)
        {
            displaySingleStudent(&students[i]);
            found++;
        }
    }

    if (!found)
        printf("No matching students found.\n");
}

static void filterByBoth(
    const Student students[],
    int count,
    const char *department,
    int semester)
{
    int found = 0;

    printf("\nDepartment: %s | Semester: %d\n",
           department, semester);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(students[i].department, department) == 0 &&
            students[i].semester == semester)
        {
            displaySingleStudent(&students[i]);
            found++;
        }
    }

    if (!found)
        printf("No matching students found.\n");
}

void displayVersion(void)
{
    printf("Student Record Management System v%s\n", VERSION);
}

void displayHelp(void)
{
    printf("\n");
    printf("============================================\n");
    printf("       SRMS COMMAND LINE INTERFACE\n");
    printf("============================================\n");

    printf("\nGENERAL COMMANDS:\n");
    printf("  srms --help\n");
    printf("  srms --version\n");

    printf("\nAUTHENTICATION:\n");
    printf("  srms --login\n");
    printf("  srms --logout\n");

    printf("\nSTUDENT MANAGEMENT:\n");
    printf("  srms --add\n");
    printf("  srms --list\n");
    printf("  srms --search ID\n");
    printf("  srms --update ID\n");
    printf("  srms --delete ID\n");
    printf("  srms --result ID\n");

    printf("\nSORTING:\n");
    printf("  srms --sort id\n");
    printf("  srms --sort name\n");
    printf("  srms --sort percentage\n");
    printf("  srms --sort total\n");

    printf("\nFILTERING:\n");
    printf("  srms --filter department CSE\n");
    printf("  srms --filter semester 2\n");
    printf("  srms --filter both CSE 2\n");

    printf("\nOTHER COMMANDS:\n");
    printf("  srms --stats\n");
    printf("  srms --export\n");

    printf("\nNote: Protected commands require an active session.\n");
    printf("============================================\n");
}

int handleCommandLine(int argc, char *argv[])
{
    Student students[MAX_STUDENTS];
    int count = 0;
    int id;

    if (argc < 2)
    {
        displayHelp();
        return 1;
    }

    /* Commands that do not require authentication */

    if (strcmp(argv[1], "--help") == 0)
    {
        if (argc != 2)
        {
            printf("Error: --help takes no arguments.\n");
            return 1;
        }

        displayHelp();
        return 0;
    }

    if (strcmp(argv[1], "--version") == 0)
    {
        if (argc != 2)
        {
            printf("Error: --version takes no arguments.\n");
            return 1;
        }

        displayVersion();
        return 0;
    }

    /* Login command */

    if (strcmp(argv[1], "--login") == 0)
    {
        if (argc != 2)
        {
            printf("Error: --login takes no arguments.\n");
            return 1;
        }

        if (validateSession())
        {
            printf("An administrator session is already active.\n");
            return 0;
        }

        if (login())
        {
            if (createSession())
                printf("Administrator session started.\n");
            else
                printf("Could not create session.\n");

            return 0;
        }

        return 1;
    }

    /* Logout command */

    if (strcmp(argv[1], "--logout") == 0)
    {
        if (argc != 2)
        {
            printf("Error: --logout takes no arguments.\n");
            return 1;
        }

        if (!validateSession())
        {
            printf("No active session. Please login first.\n");
            return 1;
        }

        logout();
        return 0;
    }

    /* All remaining commands require authentication */

    if (!validateSession())
    {
        printf("\nAccess denied!\n");
        printf("Please login using: srms --login\n");
        return 1;
    }

    /* Load student records */

    if (!loadRecords(students, &count))
    {
        printf("Error: Unable to load student records.\n");
        return 1;
    }

    /* List students */

    if (strcmp(argv[1], "--list") == 0)
    {
        if (argc != 2)
        {
            printf("Usage: srms --list\n");
            return 1;
        }

        displayStudents(students, count);
        return 0;
    }

    /* Add student */

    if (strcmp(argv[1], "--add") == 0)
    {
        if (argc != 2)
        {
            printf("Usage: srms --add\n");
            return 1;
        }

        if (addStudent(students, &count))
            saveChanges(students, count);

        return 0;
    }

    /* Search student */

    if (strcmp(argv[1], "--search") == 0)
    {
        if (argc != 3 || !parsePositiveInt(argv[2], &id))
        {
            printf("Usage: srms --search ID\n");
            return 1;
        }

        int index = findStudentById(students, count, id);

        if (index == -1)
            printf("Student with ID %d not found.\n", id);
        else
            displaySingleStudent(&students[index]);

        return 0;
    }

    /* Update student */

    if (strcmp(argv[1], "--update") == 0)
    {
        if (argc != 3 || !parsePositiveInt(argv[2], &id))
        {
            printf("Usage: srms --update ID\n");
            return 1;
        }

        if (updateStudent(students, count, id))
            saveChanges(students, count);

        return 0;
    }

    /* Delete student */

    if (strcmp(argv[1], "--delete") == 0)
    {
        if (argc != 3 || !parsePositiveInt(argv[2], &id))
        {
            printf("Usage: srms --delete ID\n");
            return 1;
        }

        if (deleteStudent(students, &count, id))
            saveChanges(students, count);

        return 0;
    }

    /* Display academic result */

    if (strcmp(argv[1], "--result") == 0)
    {
        if (argc != 3 || !parsePositiveInt(argv[2], &id))
        {
            printf("Usage: srms --result ID\n");
            return 1;
        }

        viewStudentResult(students, count, id);
        return 0;
    }

    /* Sorting */

    if (strcmp(argv[1], "--sort") == 0)
    {
        if (argc != 3)
        {
            printf("Usage: srms --sort FIELD\n");
            return 1;
        }

        if (strcmp(argv[2], "id") == 0)
            sortByID(students, count);

        else if (strcmp(argv[2], "name") == 0)
            sortByName(students, count);

        else if (strcmp(argv[2], "percentage") == 0)
            sortByPercentage(students, count);

        else if (strcmp(argv[2], "total") == 0)
            sortByTotal(students, count);

        else
        {
            printf("Invalid sorting field.\n");
            printf("Use: id, name, percentage or total\n");
            return 1;
        }

        printf("Records sorted successfully.\n");
        saveChanges(students, count);
        return 0;
    }

    /* Filtering */

    if (strcmp(argv[1], "--filter") == 0)
    {
        if (argc == 4 &&
            strcmp(argv[2], "department") == 0)
        {
            filterByDepartment(students, count, argv[3]);
            return 0;
        }

        if (argc == 4 &&
            strcmp(argv[2], "semester") == 0)
        {
            int semester;

            if (!parsePositiveInt(argv[3], &semester) ||
                semester > MAX_SEMESTER)
            {
                printf("Invalid semester number.\n");
                return 1;
            }

            filterBySemester(students, count, semester);
            return 0;
        }

        if (argc == 5 &&
            strcmp(argv[2], "both") == 0)
        {
            int semester;

            if (!parsePositiveInt(argv[4], &semester) ||
                semester > MAX_SEMESTER)
            {
                printf("Invalid semester number.\n");
                return 1;
            }

            filterByBoth(students, count, argv[3], semester);
            return 0;
        }

        printf("Usage:\n");
        printf("srms --filter department CSE\n");
        printf("srms --filter semester 2\n");
        printf("srms --filter both CSE 2\n");

        return 1;
    }

    /* Statistics */

    if (strcmp(argv[1], "--stats") == 0)
    {
        if (argc != 2)
        {
            printf("Usage: srms --stats\n");
            return 1;
        }

        displayStatistics(students, count);
        return 0;
    }

    /* Text export */

    if (strcmp(argv[1], "--export") == 0)
    {
        if (argc != 2)
        {
            printf("Usage: srms --export\n");
            return 1;
        }

        if (exportToText(students, count))
            printf("Records exported to students.txt successfully.\n");
        else
            printf("Export failed.\n");

        return 0;
    }

    printf("Unknown command: %s\n", argv[1]);
    printf("Use srms --help to see available commands.\n");

    return 1;
}