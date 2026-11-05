#include <stdio.h>
#include <string.h>
#include "auth.h"

#define USERNAME "admin"
#define PASSWORD "srms123"
#define MAX_LOGIN_ATTEMPTS 3
#define SESSION_FILE ".srms_session"

static void removeNewline(char *str)
{
    str[strcspn(str, "\n")] = '\0';
}

int login(void)
{
    char username[50];
    char password[50];

    int attempts = 0;

    while (attempts < MAX_LOGIN_ATTEMPTS)
    {
        printf("\n================================\n");
        printf("       SRMS ADMIN LOGIN\n");
        printf("================================\n");

        printf("Username: ");
        if (fgets(username, sizeof(username), stdin) == NULL)
            return 0;

        removeNewline(username);

        printf("Password: ");
        if (fgets(password, sizeof(password), stdin) == NULL)
            return 0;

        removeNewline(password);

        if (strcmp(username, USERNAME) == 0 &&
            strcmp(password, PASSWORD) == 0)
        {
            printf("\nLogin successful!\n");
            return 1;
        }

        attempts++;
        printf("\nInvalid credentials! Attempts remaining: %d\n",
               MAX_LOGIN_ATTEMPTS - attempts);
    }

    printf("\nMaximum login attempts reached.\n");
    return 0;
}

int createSession(void)
{
    FILE *fp = fopen(SESSION_FILE, "w");

    if (fp == NULL)
    {
        printf("Error: Unable to create session.\n");
        return 0;
    }

    fprintf(fp, "SRMS_SESSION_ACTIVE\n");

    if (fclose(fp) != 0)
        return 0;

    return 1;
}

int validateSession(void)
{
    FILE *fp = fopen(SESSION_FILE, "r");
    char status[50];

    if (fp == NULL)
        return 0;

    if (fgets(status, sizeof(status), fp) == NULL)
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);
    removeNewline(status);

    return strcmp(status, "SRMS_SESSION_ACTIVE") == 0;
}

int destroySession(void)
{
    if (remove(SESSION_FILE) == 0)
        return 1;

    return 0;
}

void logout(void)
{
    if (validateSession())
        destroySession();

    printf("\nSuccessfully logged out.\n");
}