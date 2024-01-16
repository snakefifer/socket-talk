#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "syscalls.h"
#include "globalDefs.h"
#include "serverFuncs.h"

int main(int argc, char* argv[]) {
    char hostname[256];
    gethostname(hostname, 255);
    strcat(hostname, ".cse.lehigh.edu");
    int port = parsePort(argc, argv);
    printf("Hostname:Port - %s:%d\n", hostname, port);
    int acceptFD = acceptConnection(port);
    printf("Accepted Connection.\n");
    int size = 256;
    char *message = (char *)Malloc(size);
    char errMsg[256] = "Invalid response, try again.\n";
    strcpy(message, "Pick a number that correlates to the file:\n1. csjobs\n2. urandom\n3. wealth\n4. exit\n");
    sendPrompt(acceptFD, message);
    do {  
        char *recMessage = receiveMessage(acceptFD, message, size);
        recMessage[strlen(recMessage) - 1] = '\0';
        if (strcmp(recMessage, "1") == 0 || strcmp(recMessage, "csjobs") == 0) {
            strcpy(recMessage, "csjobs");
            handleLogic(acceptFD, recMessage, "text", message);
            break;
        } else if (strcmp(recMessage, "2") == 0 || strcmp(recMessage, "urandom") == 0) {
            strcpy(recMessage, "urandom");
            handleLogic(acceptFD, recMessage, "binary", message);
            break;
        } else if (strcmp(recMessage, "3") == 0 || strcmp(recMessage, "wealth") == 0) {
            strcpy(recMessage, "wealth");
            handleLogic(acceptFD, recMessage, "text", message);
            break;
        } else {
            printf("Invalid response: %s\n", recMessage);
            sendPrompt(acceptFD, errMsg);
        }
    } while (1);
    Close(acceptFD);
    return 0;
}