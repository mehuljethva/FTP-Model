/*
 Mehul & Rahil
 Client side implementation to fire commands and establish a connection to server.
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
#define MAXLINE 1024

char buf[1024];
char buffer[1024];
char file_name[100];

int sockfd;
// Function to store file received from server to the client file system 
void store_file(char *dest, char file[1024]) {

	FILE *fp1;
	fp1 = fopen(dest, "w");
	fprintf(fp1,"%s",file);
	fclose(fp1);
}
// Split function to set the file name.
void split(char *pathname)
{

	char splitStrings[10][10];
	int i, j, cnt;

	j = 0;
	cnt = 0;
	for (i = 0; i <= (strlen(pathname)); i++)
	{
		if (pathname[i] == '/' || pathname[i] == '\0')
		{
			splitStrings[cnt][j] = '\0';
			cnt++; // for next word
			j = 0; // for next word, init index to 0
		}
		else
		{
			splitStrings[cnt][j] = pathname[i];
			j++;
		}
	}
	strcpy(file_name, splitStrings[cnt - 1]);
	file_name[strlen(file_name)] = '\0';
}

// Retrieving file from server
void retrieve_file(char *pathname, char *buffer, int sockfd) {

	pathname = pathname+5;
	split(pathname);
	
	if(strncmp(buffer,"retr",4) == 0) {
		printf("%s \n", file_name);	
		
		char res[1024];
		char* temp[1024];
		bzero(buffer, sizeof(buffer));
			
		recv(sockfd, buffer, MAXLINE, 0);
			
		strcpy(res, buffer);
		bzero(buffer, sizeof(buffer));

		recv(sockfd, buffer, MAXLINE,0);

		getcwd(buf,256);
    	strcat(buf,"/");
		strtok(file_name,"\n");
		strcat(buf, file_name);
		bzero(buffer, sizeof(buffer));
		store_file(buf, res);

		// printf("File-Name: %s.\n", file_name);
		// printf("File-Size: %ld bytes.\n", strlen(res));
		printf("File Retrived Successfully.\n");
		// printf("FILE OK...Transfer Completed.");
		printf("\n");
	}
}

// Trial implementation for receiving file from server
// void receive_file(char *buf, int sockfd)
// {
// 	int n;
// 	FILE *fp;
// 	char *filename;
// 	recv(sockfd, buf, SIZE, 0);
// 	// printf("");
// 	filename = "receivedbyclient.txt";
// 	char buffer[SIZE];
// 	fp = fopen(filename, "w");
// 	fprintf(fp, "%s", buf);
// 	fclose(fp);
// 	bzero(buf, sizeof(buf));
// 	printf("File retrieved successfully \n");
// 	return;
// }

// Stop FTP connection from client end.
void quit_command()
{
	printf("\e]2;FTP-Client\a \n");
	printf("%s \n", buffer);
	printf("[+]Closing the connection.\n");
	close(sockfd);
	exit(1);
}

// Send file to server for upload / stor command
void send_file(char *buf, int sockfd) {
		
	char file[1024];
	char ch;
	char file_path[1024];
	int k = 0;
			
	for(int i=5; i < strlen(buf); i++) {
		file_path[k] = buf[i];
		k++;
	}
	strtok(file_path,"\n");


	FILE *fp;
	if((fp = fopen(file_path, "r")) != NULL) {
        		
        k = 0;
        ch = fgetc(fp);
    	while (ch != EOF) {
        	file[k] = ch;
        	k++;
        	ch = fgetc(fp);
    	}
    	file[k] = '\0';
    	fclose(fp);
    			
    	printf("Source-File-Path : %s\n", file_path);
    	printf("File-Size        : %ld bytes\n", strlen(file));

    	strcpy(buffer, file);
    	send(sockfd, buffer,MAXLINE, 0);
    	bzero(buffer, sizeof(buffer));
    }
    
    else {
			
		if(errno == ENOENT) {
            printf("Invalid Filename: File does not exist.\n");
        }
        else
        	printf("Error opening file.\n");
        send(sockfd, "NULL", 4, 0);
    }
}


int main()
{
	char *ip = "127.0.0.1";
	int port = 8080;
	

	int sockfd;
	struct sockaddr_in server_addr;

	// cerate a socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		perror("[-]Error in socket creation");
		exit(1);
	}
	printf("[+]Client socket created successfully.\n");
	
	// specify an address for socket
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	int connection_status = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)); // connect initiates three-way handshaking

	if (connection_status == -1)
	{
		perror("[-]Error in socket");
		exit(1);
	}
	printf("[+]Connected to Server.\n");
	
	printf("\033[0;32m");
    printf("--->>>Welcome to TCP connection.. \n List of commands:\n1.User\n2.PWD\n3.CWD <path-of-directory>\n4.List <path-of-directory>\n5.CDUP <path-of-directory>\n6.STOR <path-of-directory>\n7.RETR <path-of-directory>\n8.PORT <port-number>\n9.DELE <path-of-file>\n10.QUIT\n11.NOOP\n");
    printf("\033[0;37m");
	
	printf("\n");
	while (1)
	{
		printf("Enter Command:-\n");
		fgets(buffer, 1024, stdin); // take input from command lines
		buffer[strlen(buffer)] = '\0';
		send(sockfd, buffer, strlen(buffer), 0);
		strcpy(buf, buffer);
		buf[strlen(buf)] = '\0';
		bzero(buffer, sizeof(buffer));

		if (strncmp(buf, "STOR", 4) == 0 || strncmp(buf, "stor", 4) == 0)
		{
			send_file(buf, sockfd);
			continue;
		}

		bzero(buffer, sizeof(buffer));
		if (recv(sockfd, buffer, 1024, 0) < 0)
		{
			printf("Error in receiving.\n");
			exit(1);
		}

		else if(strncmp(buf, "RETR", 4) == 0 || strncmp(buf, "retr", 4) == 0){
			retrieve_file(buf, buffer, sockfd);	
		}

		else if (strncmp(buf, "QUIT", 4) == 0 || strncmp(buf, "quit", 4) == 0){
			quit_command(); 
		}
		
		else{
			printf("%s\n", buffer);
		}
		bzero(buffer, sizeof(buffer));
	}

close(sockfd);
return 0;
}