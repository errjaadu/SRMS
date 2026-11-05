#ifndef AUTH_H
#define AUTH_H

int login(void);
void logout(void);

int createSession(void);
int validateSession(void);
int destroySession(void);

#endif