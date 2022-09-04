/*
 Mehul & Rahil
 
*/
#include "header.h"
void retrive_file(char *buf, int sockfd)
{
    // buf = buf+5;
    char file[1024];
    char ch;
    char fname[1024];
    int k = 0;
    FILE *fp;
    send(sockfd, buf, MAXLINE, 0);
    bzero(buf, sizeof(buf));
    recv(sockfd, buf, MAXLINE, 0);

    if ((fp = fopen(buf, "r")) != NULL)
    {
        k = 0;
        ch = fgetc(fp);

        while (ch != EOF)
        {
            file[k] = ch;
            k++;
            ch = fgetc(fp);
        }

        file[k] = '\0';
        fclose(fp);
        bzero(buf, sizeof(buf));
        strcpy(buf, file);
        send(sockfd, buf, SIZE, 0);
        bzero(buf, sizeof(buf));
    }
    return;
}