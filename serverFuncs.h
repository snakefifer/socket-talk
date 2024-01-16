#ifndef SERVERFUNCS_H
#define SERVERFUNCS_H

#include "globalDefs.h"
#include <stdio.h>

int parsePort(int argc, char *argv[]);
int acceptConnection(int port);
int sendPrompt(int acceptFD, char msg[]);
char* receiveMessage(int acceptFD, char *msg, int size);
void crypto(char *buffer);
char* fullPath(char *start);
unsigned long hash(char *path, const char *type);
char* makeJson(char *fileNname, ssize_t fileSize, const char *text, unsigned long signature);
ssize_t sizeOfFile(FILE *fp);
char* waitReady(int acceptFD, char *msg, int size);
long sendFile(int sockFD, char *path, const char *fileType);
void handleLogic(int acceptFD, char *recMessage, const char *fileType, char *message);

#endif