#include "statistics.h"
#include <stdio.h>
#include <string.h>

void displayStatistics(const Student students[], int count)
{
    const char *departments[] = {"CSE", "ECE", "ME", "CE", "EE"};
    const char *grades[] = {"α+", "α", "α−", "β", "γ", "F"};
    int departmentCounts[5] = {0};
    int gradeCounts[6] = {0};
    int i, j, gradeIndex;
    float sum = 0.0f, highest = 0.0f, lowest = 100.0f;

    if (count == 0)
    {
        printf("\nNo records available for statistics.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        sum += students[i].percentage;
        if (students[i].percentage > highest) highest = students[i].percentage;
        if (students[i].percentage < lowest) lowest = students[i].percentage;

        for (j = 0; j < 5; j++)
            if (strcmp(students[i].department, departments[j]) == 0)
                departmentCounts[j]++;

        gradeIndex = 5;
        for (j = 0; j < 5; j++)
            if (strcmp(students[i].grade, grades[j]) == 0)
            {
                gradeIndex = j;
                break;
            }
        gradeCounts[gradeIndex]++;
    }

    printf("\n============= STATISTICS DASHBOARD =============\n");
    printf("Total students       : %d\n", count);
    printf("Average percentage  : %.2f%%\n", sum / count);
    printf("Highest percentage  : %.2f%%\n", highest);
    printf("Lowest percentage   : %.2f%%\n", lowest);

    printf("\nDepartment-wise count:\n");
    for (i = 0; i < 5; i++)
        printf("  %-5s : %d\n", departments[i], departmentCounts[i]);

    printf("\nGrade distribution:\n");
    for (i = 0; i < 6; i++)
        printf("  %-4s : %d\n", grades[i], gradeCounts[i]);
    printf("=================================================\n");
}
