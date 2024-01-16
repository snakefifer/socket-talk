#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <dirent.h>        /* for struct dirent */
#include <time.h>          /* for struct timespec */
#include <sys/socket.h>    /* for socklen_t */
#include <stdio.h>         /* for FILE type */

ssize_t Recv(int socket, void *buffer, size_t length, int flags);
ssize_t Send(int socket, const void *buffer, size_t length, int flags);
int Accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
int Connect(int socket, const struct sockaddr *address, socklen_t address_len);
int Listen(int socket, int backlog);
int Bind(int socket, const struct sockaddr *address, socklen_t address_len);
int Socket(int domain, int type, int protocol);
int Socketpair(int domain, int type, int protocol, int socket_vector[2]);
pid_t Wait(int *wstatus);
int Pipe(int fildes[2]);
int Nanosleep(const struct timespec *req, struct timespec *rem);
pid_t Fork(void);
void *Malloc(size_t size);
ssize_t Read(int fildes, void *buf, size_t nbyte);
ssize_t Write(int fildes, const void *buf, size_t nbyte);
int Close(int fildes);
int Chdir(const char *path);
struct dirent *Readdir(DIR *dirp);
DIR *Opendir(const char *name);
int Closedir(DIR *dirp);
FILE *Fopen(const char *path, const char *mode);
void Fclose(FILE *fp);
int Fseek(FILE *fp, long offset, int whence);
long Ftell(FILE *fp);
size_t Fread(void *ptr, size_t item_size, size_t num_items, FILE *fp);
size_t Fwrite(const void *ptr, size_t item_size, size_t num_items, FILE *fp); 

#endif