/*
 Mehul & Rahil
 FTP Server socket creation & binding.
*/
#include "header.h"

#include "getcmdAndProceed.c"

int main() {

     char *ip = "127.0.0.1";
     int port = 8080;
     int connection_status;
     int sockfd, client_socket;
     struct sockaddr_in server_addr, new_addr; // server socket add and client socket add structure
     socklen_t addr_size; // value result argument
     char buffer[SIZE];
     char buf[MAXLINE];

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     // Problem in creating server socket.
     if(sockfd < 0) {
          perror("[-]Error in socket");
          exit(1);
     }
     
     printf("[+]Server socket created successfully.\n");
     // specify an address for socket
     server_addr.sin_family = AF_INET; // IPv4
     server_addr.sin_port = port;
     server_addr.sin_addr.s_addr = inet_addr(ip); //Server IP-address of '127.0.0.1'
     
     int optval = 1;
     setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));  // To reuse same Port multiple times
     
     connection_status = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

     if(connection_status < 0) {
          perror("[-]Error in bind");
          exit(1);
     }
     printf("[+]Binding successfull to .\n");

     

     if(listen(sockfd, 10) == 0){
          printf("[+]Listening....\n");
     } else {
          perror("[-]Error in listening");
          exit(1);
     }    
     // Loop for the server to listen continuously.
     while (1)
     {
          client_socket = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
          
          if(client_socket < 0) {
               exit(1);
          }
          // Run the command received from client.
          if (fork() == 0 ) {
               close(sockfd);
               while(1) {
                    bzero(buf, sizeof(buf));
                    if(recv(client_socket, buf, MAXLINE, 0) < 0) {     
                         char temp[MAXLINE];
                         *temp = '\0';
                         strcat(temp, "Data Connection Open but No Transfer in progress.");
                         send(client_socket, temp, MAXLINE, 0);
                         continue;
                    }
               // Parse the command received from client's socket and its implementation
               getCmdAndProceed(buf, client_socket);   
               // Worked for retr flag
               if (command==RETR) {
                    
                    char temp[1024];
                    strcpy(temp, "retr");
                    send(client_socket, temp, MAXLINE, 0);
                    
                    send(client_socket, file, MAXLINE, 0);
                    printf(">> File Sent Successfully.\n");
                    
                    strtok(temp, "\n");
                    send(client_socket, temp, MAXLINE, 0);
                }           
                   
               }
               
               bzero(buf, sizeof(buf));
          }
     }
// Close the socket from client.     
close(client_socket);
return 0;
}