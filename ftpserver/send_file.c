/*
 Mehul & Rahil
 Code for transfering file from server to client in the retr command
*/
#include "header.h"
// Implementation for sending file to the client using socket. 
void send_file(char *filename, int sockfd) {
		
    strtok(filename, "\n");
    
    char ch;
	FILE *fp1;
    
    if((fp1 = fopen(filename, "r")) == NULL) {
        if(errno == ENOENT) {
            printf("347 \n");
            return;
        }
        printf("348 \n");
        return ;
    }
    
    int k = 0;
    ch = fgetc(fp1);
    while (ch != EOF) {
        file[k] = ch;
        k++;
        ch = fgetc(fp1);
    }
    file[k] = '\0';
    return;
}