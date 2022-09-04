/*
 Mehul & Rahil
 Common header file for the FTP using sockets to run in sync.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/dir.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <libgen.h>

#define SIZE 1024
#define USER 1
#define QUIT 11
#define RETR 10
#define MAXLINE 1024

char list[1024];
char file_name[100];
char pwdbuf[256];
int users[100];
struct stat st;
char buffer[1024];
char file[1024];
int command;
void split(char *pathname);
int store_file(char *pathname, int sockfd);
// void retrive_file(char *buf, int sockfd);