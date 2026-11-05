#include <stdio.h>
#include <stdlib.h>

#include "student.h"
#include "student_ops.h"
#include "auth.h"
#include "cli.h"
#include "file_handling.h"
#include "sorting.h"
#include "statistics.h"

static int readMenuChoice(void)
{
    char buffer[30];
    char *end;
    long choice;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return -1;

    choice = strtol(buffer, &end, 10);

    while (*end == ' ' || *end == '\t')
        end++;

    if (end == buffer || (*end != '\n' && *end != '\0'))
        return -1;

    return (int)choice;
}

static void saveAndExport(Student students[], int count)
{
    if (saveRecords(students, count))
    {
        printf("Binary records saved successfully.\n");

        if (exportToText(students, count))
            printf("Text export updated successfully.\n");
        else
            printf("Warning: Text export failed.\n");
    }
    else
    {
        printf("Error: Unable to save records.\n");
    }
}

static void runInteractiveMode(void)
{
    Student students[MAX_STUDENTS];
    int count = 0;
    int choice;

    if (!login())
        return;

    if (!loadRecords(students, &count))
    {
        printf("Unable to load student records.\n");
        return;
    }

    printf("\nRecords loaded: %d\n", count);

    do
    {
        printf("\n");
        printf("========================================\n");
        printf("     STUDENT RECORD MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("1.  Add New Student\n");
        printf("2.  Display All Students\n");
        printf("3.  Search Student\n");
        printf("4.  Update Student\n");
        printf("5.  Delete Student\n");
        printf("6.  Academic Result\n");
        printf("7.  Sort Records\n");
        printf("8.  Filter Records\n");
        printf("9.  Statistics Dashboard\n");
        printf("10. Export Records\n");
        printf("11. Logout and Exit\n");
        printf("----------------------------------------\n");
        printf("Enter choice: ");

        choice = readMenuChoice();

        switch (choice)
        {
            case 1:
                if (addStudent(students, &count))
                    saveAndExport(students, count);
                break;

            case 2:
                displayStudents(students, count);
                break;

            case 3:
            {
                int id;
                printf("Enter student ID: ");

                if (scanf("%d", &id) == 1)
                {
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}

                    int index = findStudentById(students, count, id);

                    if (index == -1)
                        printf("Student not found.\n");
                    else
                        displaySingleStudent(&students[index]);
                }
                else
                {
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    printf("Invalid ID.\n");
                }
                break;
            }

            case 4:
            {
                int id;
                printf("Enter student ID to update: ");

                if (scanf("%d", &id) == 1)
                {
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}

                    if (updateStudent(students, count, id))
                        saveAndExport(students, count);
                }
                else
                {
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    printf("Invalid ID.\n");
                }
                break;
            }

            case 5:
            {
                int id;
                printf("Enter student ID to delete: ");

                if (scanf("%d", &id) == 1)
                {
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}

                    if (deleteStudent(students, &count, id))
                        saveAndExport(students, count);
                }
                else
                {
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    printf("Invalid ID.\n");
                }
                break;
            }

            case 6:
            {
                int id;
                printf("Enter student ID: ");

                if (scanf("%d", &id) == 1)
                {
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    viewStudentResult(students, count, id);
                }
                else
                {
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {}
                    printf("Invalid ID.\n");
                }
                break;
            }

            case 7:
            {
                int sortChoice;

                printf("\n1. Sort by ID\n");
                printf("2. Sort by Name\n");
                printf("3. Sort by Percentage\n");
                printf("4. Sort by Total Marks\n");
                printf("Enter choice: ");

                sortChoice = readMenuChoice();

                switch (sortChoice)
                {
                    case 1: sortByID(students, count); break;
                    case 2: sortByName(students, count); break;
                    case 3: sortByPercentage(students, count); break;
                    case 4: sortByTotal(students, count); break;
                    default:
                        printf("Invalid sorting choice.\n");
                        continue;
                }

                saveAndExport(students, count);
                printf("Records sorted successfully.\n");
                break;
            }

            case 8:
                filterStudents(students, count);
                break;

            case 9:
                displayStatistics(students, count);
                break;

            case 10:
                if (exportToText(students, count))
                    printf("Export completed successfully.\n");
                else
                    printf("Export failed.\n");
                break;

            case 11:
                printf("\nSaving records before exit...\n");
                saveAndExport(students, count);
                logout();
                printf("Thank you for using SRMS!\n");
                break;

            default:
                printf("Invalid menu choice. Try again.\n");
        }

    } while (choice != 11);
}

int main(int argc, char *argv[])
{
    if (argc > 1)
        return handleCommandLine(argc, argv);

    runInteractiveMode();

    return 0;
}