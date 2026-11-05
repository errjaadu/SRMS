#ifndef STUDENT_H
#define STUDENT_H

#define MAX_STUDENTS 500
#define SUBJECTS 5
#define MAX_NAME 50
#define MAX_SUBJECT_NAME 30
#define MAX_DEPARTMENT 30
#define MAX_COURSE 30
#define MAX_PHONE 15
#define MAX_GENDER 10
#define MAX_SEMESTER 8

typedef struct
{
    int id;
    char name[MAX_NAME];
    int age;
    char gender[MAX_GENDER];
    char course[MAX_COURSE];
    char department[MAX_DEPARTMENT];
    int semester;
    char phone[MAX_PHONE];
    char subjectNames[SUBJECTS][MAX_SUBJECT_NAME];
    float marks[SUBJECTS];
    float total;
    float percentage;
    char grade[10];
} Student;

#endif
