#include <stdio.h>       /* for fprintf/fopen/fclose/fseek/ftell/fread/fwrite */
#include <stdlib.h>      /* for exit/malloc */
#include <errno.h>       /* for errno */
#include <string.h>      /* for strerror */
#include <unistd.h>      /* for chdir/read/write/pipe/fork/close */
#include <sys/types.h>   /* for opendir/closedir*/
#include <dirent.h>      /* for opendir/closedir/readdir */
#include <time.h>        /* for nanosleep */
#include <sys/socket.h>  /* for socketpair/socket/bind/listen/connect/accept/send */
#include <sys/wait.h>    /* for wait */

int Chdir(const char *path) {
  if (chdir(path)) {
    fprintf(stderr, "chdir error (%s) - %s\n", path, strerror(errno));
    exit(errno);
  }
  return 0;
}

struct dirent *Readdir(DIR *dirp) {
  errno = 0;
  struct dirent *filep = NULL;
  if (!(filep = readdir(dirp)) && errno) {
    fprintf(stderr, "readdir error - %s\n", strerror(errno));
    exit(errno);
  }
  return filep;
}

DIR *Opendir(const char *name) {
  DIR *dirp = NULL;
  if (!(dirp = opendir(name))) {
    fprintf(stderr, "opendir error (%s) - %s\n", name, strerror(errno)); 
    exit(errno);
  }
  return dirp;
}

int Closedir(DIR *dirp) {
  if (closedir(dirp)) {
    fprintf(stderr, "closedir error - %s\n", strerror(errno));
    exit(errno);
  }
  return 0;
}

FILE *Fopen(const char *path, const char *mode) {
  FILE *fp = NULL;
  if ((fp = fopen(path, mode)) == NULL) {
    fprintf(stderr, "Unable to open %s with mode %s - %s\n", path, mode, strerror(errno));
    exit(errno);
  }  
  return fp; 
} 

void Fclose(FILE *fp) {
  if (fp && fclose(fp)) {
    fprintf(stderr, "Unable to close file descriptor %d - %s\n", fileno(fp), strerror(errno));
    exit(errno);
  }
}

int Fseek(FILE *fp, long offset, int whence) {
  int newOffset;
  if ((newOffset = fseek(fp, offset, whence)) == -1) {
    fprintf(stderr, "Unable to seek using offset %ld with whence %d - %s\n", offset, whence, strerror(errno));
    exit(errno);
  }
  return newOffset;
}

long Ftell(FILE *fp) {
  long offset;
  if ((offset = ftell(fp)) == -1) {
    fprintf(stderr, "Unable to obtain position using ftell - %s\n", strerror(errno));
    exit(errno);
  }
  return offset;
}

size_t Fread(void *ptr, size_t item_size, size_t num_items, FILE *fp) {
  size_t items_read = 0;
  if ((items_read = fread(ptr, item_size, num_items, fp)) == 0)
    if (!feof(fp) && ferror(fp)) {
      fprintf(stderr, "Encountered error while reading from file descriptor %d - %s\n", fileno(fp), strerror(errno));
      exit(errno);
    }   
  return items_read;
}

size_t Fwrite(const void *ptr, size_t item_size, size_t num_items, FILE *fp) { 
  size_t items_written = 0;
  if ((items_written = fwrite(ptr, item_size, num_items, fp)) == 0)
    if (ferror(fp)) {
      fprintf(stderr, "Encountered error while writing to file descriptor %d - %s\n", fileno(fp), strerror(errno));
      exit(errno);
    }   
  return items_written;
}

void *Malloc(size_t size) {
  void *ptr;
  if ((ptr = malloc(size)) == NULL) {
    fprintf(stderr, "Could not allocate space - %s\n", strerror(errno));
    exit(errno);
  }
  return ptr;
}

ssize_t Read(int fildes, void *buffer, size_t nbytes) {
  ssize_t numBytes;
  if ((numBytes = read(fildes, buffer, nbytes)) == -1) {
    fprintf(stderr, "read error on descriptor %d - %s\n", fildes, strerror(errno));
    exit(errno);
  }
  return numBytes;
}

ssize_t Write(int fildes, const void *buffer, size_t nbytes) {
  ssize_t numBytes;
  if ((numBytes = write(fildes, buffer, nbytes)) == -1) {
    fprintf(stderr, "Write error on descriptor %d - %s\n", fildes, strerror(errno));
    exit(errno);
  }
  return numBytes;
}

int Pipe(int pfd[2]) {
  if (pipe(pfd) == -1) {
    fprintf(stderr, "Pipe error %d - %s\n", errno, strerror(errno));
    exit(errno);
  }
  return 0;
}

int Nanosleep(const struct timespec *req, struct timespec *rem) {
  if (nanosleep(req, rem) == -1) {
    fprintf(stderr, "Nanosleep error %d - %s\n", errno, strerror(errno));
    exit(errno);
  }
  return 0;
}

pid_t Fork(void) {
  pid_t pid;
  if ((pid = fork()) == -1) {
    fprintf(stderr, "Fork error %d - %s\n", errno, strerror(errno));
    exit(errno);
  }
  return pid;
}

int Close(int pfd) {
  if (close(pfd) == -1) {
    fprintf(stderr, "Close error %d - %s\n", errno, strerror(errno));
    exit(errno);
  }
  return 0;
}

int Socketpair(int domain, int type, int protocol, int socket_vector[2]) {
  if (socketpair(domain, type, protocol, socket_vector) == -1) {
    fprintf(stderr, "Socketpair could not be created with domain %d, type %d, and protocol %d - %s\n", domain, type, protocol, strerror(errno));
    exit(errno);
  }
  return 0;
}

pid_t Wait(int *stat_loc) {
  pid_t pid;
  if ((pid = wait(stat_loc)) == -1) {
    fprintf(stderr, "Wait error - %s\n", strerror(errno));
    exit(errno);
  }
  return pid;
}

int Accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len) {
  int filde = accept(socket, address, address_len);
  if (filde == -1) {
    fprintf(stderr, "accept error - %s\n", strerror(errno));
    exit(errno);
  }
  return filde;
}

int Connect(int socket, const struct sockaddr *address, socklen_t address_len) {
  if ((connect(socket, address, address_len)) == -1) {
    fprintf(stderr, "connect error - %s\n", strerror(errno));
    exit(errno);
  }
  return 0;
}

int Listen(int socket, int backlog) {
  if ((listen(socket, backlog)) == -1) {
    fprintf(stderr, "listen error - %s\n", strerror(errno));
    exit(errno);
  }
  return 0;
}

int Bind(int socket, const struct sockaddr *address, socklen_t address_len) {
  if ((bind(socket, address, address_len)) == -1) {
    fprintf(stderr, "bind error - %s\n", strerror(errno));
    exit(errno);
  }
  return 0;
}

int Socket(int domain, int type, int protocol) {
  int filde = socket(domain, type, protocol);
  if (filde == -1) {
    fprintf(stderr, "socket error - %s\n", strerror(errno));
    exit(errno);
  }
  return filde;
}

ssize_t Send(int socket, const void *buffer, size_t length, int flags) {
  ssize_t sent = send(socket, buffer, length, flags);
  if (sent == -1) {
    fprintf(stderr, "send error - %s\n", strerror(errno));
    exit(errno);
  }
  return sent;
}

ssize_t Recv(int socket, void *buffer, size_t length, int flags) {
  ssize_t read = recv(socket, buffer, length, flags);
  if (read == -1) {
    fprintf(stderr, "recv error - %s\n", strerror(errno));
    exit(errno);
  }
  return read;
}