/*
 Mehul & Rahil
 Implementation for stor command to upload file from client machine to server's machine.
*/
#include "header.h"

int store_file(char *pathname, int sockfd) {

    char buf[1024];
    char res[1024];
    recv(sockfd, buf, MAXLINE, 0);

    if (strncmp(buf,"NULL",4) == 0) {
        printf("Error in File name.\n");
        return -1;
    }

    strcpy(res, buf);
    bzero(buf, sizeof(buf));

    split(pathname);
    strtok(pathname,"\n");
    char dest[256];
    getcwd(dest,256);
    strcat(dest,"/");
    strtok(file_name,"\n");
    strcat(dest, file_name);
    dest[strlen(dest)] = '\0';

    FILE *fp1;
    fp1 = fopen(dest, "w");
    fprintf(fp1,"%s",res);
    fclose(fp1);
    
    return 0;
}