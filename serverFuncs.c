#define _GNU_SOURCE
#include "serverFuncs.h"
#include <sys/socket.h> 
#include <netinet/in.h>
#include "syscalls.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int parsePort(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid Argument Count.\n");
        exit(99);
    }
    int port = atoi(argv[1]);
    if (port == 0) {
        printf("Invalid Port.\n");
        exit(99);
    }
    return port;
}

int acceptConnection(int port) {
    int listenFD, acceptFD;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddr_len;
    listenFD = Socket(AF_INET, SOCK_STREAM, 0);    
    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);
    Bind(listenFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    Listen(listenFD, 1);
    acceptFD = Accept(listenFD, (struct sockaddr*)&clientAddr, &clientAddr_len);
    Close(listenFD);
    return acceptFD;
}

int sendPrompt(int acceptFD, char msg[]) {
    char *msg_ptr = &msg[0];
    int bytes_xferd, bytes_left;
    bytes_left = strlen(msg) + 1;
    while (bytes_left > 0) {
        bytes_xferd = Send(acceptFD, msg_ptr, bytes_left, 0);
        bytes_left -= bytes_xferd;
        msg_ptr += bytes_xferd;
        if (bytes_left > 0)
            printf("Error sending message.\n");
        else
            printf("Message sent.\n");
    }
    return 0;
}

char* receiveMessage(int acceptFD, char *msg, int size) {
    memset(msg, '\0', size);
    int bytes = Recv(acceptFD, msg, size - 1, 0);
    msg[bytes] = '\0';
    return msg;
}

void crypto(char *buffer) {
    int length = strlen(buffer) - 1;
    for (int i = 0; i < length; i++) {
        buffer[i] ^= ENCRYPTION_KEY[i % (strlen(ENCRYPTION_KEY))]; 
    }
}

char* fullPath(char *start) {
    char *path = (char *)Malloc(256);
    strcpy(path, "/path/to/be/concat");
    strcat(path, start);
    return path;
}

unsigned long hash(char *path, const char *type) {
    FILE *file;
    if (strcmp(type, "binary") == 0)
        file = Fopen(path, "rb");
    else
        file = Fopen(path, "r");
    unsigned long hash = 5381;
    int c;
    while ((c = fgetc(file)) != EOF)
        hash = ((hash << 5) + hash) ^ c;
    Fclose(file);
    return hash;
}

char* makeJson(char *fileName, ssize_t fileSize, const char *text, unsigned long signature) {
    char *out;
    asprintf(&out, "{ \"file_name\": \"%s\", \"file_size\": %zd, \"file_type\": \"%s\", \"file_signature\": %lu }\n", fileName, fileSize, text, signature);
    return out;
}

ssize_t sizeOfFile(FILE *fp) {
    ssize_t size;
    Fseek(fp, 0L, SEEK_END);
    size = Ftell(fp);
    return size;
}

char* waitReady(int acceptFD, char *msg, int size) {
    char *out = receiveMessage(acceptFD, msg, size);
    if (strcmp(out, "ready") != 0) {
        printf("unable to validate message\n");
    }
    return out;
}

long sendFile(int sockFD, char *path, const char *fileType) {
    char *buffer = (char *)Malloc(ITEMS_TO_COPY);
    FILE *base;
    if (strcmp(fileType, "binary") == 0)
        base = Fopen(path, "rb");
    else
        base = Fopen(path, "r");
    size_t readCount = 0;
    long totalBytes = 0;
    while ((readCount = Fread(buffer, 1, ITEMS_TO_COPY, base)) > 0) {
        Send(sockFD, buffer, readCount, 1);
        totalBytes += readCount;
    }
    Send(sockFD, buffer, 1, 1);
    Fclose(base);
    free(buffer);
    return totalBytes;
}

void handleLogic(int acceptFD, char *recMessage, const char *fileType, char *message) {
    char *path = fullPath(recMessage);
    char path2[256];
    strcpy(path2, path);
    FILE *file;
    if (strcmp(fileType, "binary") == 0)
        file = Fopen(path, "rb");
    else
        file = Fopen(path, "r");
    ssize_t size = sizeOfFile(file);
    unsigned long hashed = hash(path, fileType);
    char *json = makeJson(recMessage, size, fileType, hashed);
    printf("%s", json);
    sendPrompt(acceptFD, json);
    waitReady(acceptFD, message, size);
    sendFile(acceptFD, path2, fileType);
    printf("Sent file.");
}