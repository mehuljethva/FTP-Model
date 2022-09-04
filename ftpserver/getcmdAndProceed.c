/*
 Mehul & Rahil
 Implementation for parsing the command received from the client's end.
*/
#include "header.h"
#include "split.c"
// #include "retrive_file.c"
#include "store_file.c"
#include "send_file.c"

int command = USER;
int counter = 0;

char *dirname(char *path)
{

    static const char dot[] = ".";
    char *last_slash;

    /* Find last '/'.  */
    last_slash = path != NULL ? strrchr(path, '/') : NULL;

    if (last_slash == path)
        /* The last slash is the first character in the string.  We have to
             return "/".  */
        ++last_slash;
    else if (last_slash != NULL && last_slash[1] == '\0')
        /* The '/' is the last character, we have to look further.  */
        last_slash = memchr(path, last_slash - path, '/');

    if (last_slash != NULL)
        /* Terminate the path.  */
        last_slash[0] = '\0';
    else
        /* This assignment is ill-designed but the XPG specs require to
             return a string containing "." in any case no directory part is
             found and so a static and constant string is required.  */
        path = (char *)dot;

    return path;
}
// Implementation of various FTP commands received from client.
int getCmdAndProceed(char *buf, int sock)
{

    while (*buf == ' ' || *buf == '\t')
    {
        buf++;
    }

    if (strncmp(buf, "USER ", 5) == 0 || strncmp(buf, "user ", 5) == 0 ||
        strncmp(buf, "USER\t", 5) == 0 || strncmp(buf, "user\t", 5) == 0 ||
        strncmp(buf, "USER\n", 5) == 0 || strncmp(buf, "user\n", 5) == 0)
    {
        // return ok message.
        strcpy(buf, "230 User logged in, proceed");
        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    // socket sends current working directory to client
    else if (strncmp(buf, "PWD ", 4) == 0 || strncmp(buf, "pwd ", 4) == 0 ||
             strncmp(buf, "PWD\t", 4) == 0 || strncmp(buf, "pwd\t", 4) == 0 ||
             strncmp(buf, "PWD\n", 4) == 0 || strncmp(buf, "pwd\n", 4) == 0)
    {
        if (getcwd(pwdbuf, 256) == NULL)
        {
            strcpy(buf, "342 Error in PWD");
            send(sock, buf, MAXLINE, 0);
            return 0;
        }
        strtok(pwdbuf, "\n");
        strcpy(buf, pwdbuf);
        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    // change the current working directory for specific client.
    else if (strncmp(buf, "CWD ", 4) == 0 || strncmp(buf, "cwd ", 4) == 0 ||
             strncmp(buf, "CWD\t", 4) == 0 || strncmp(buf, "cwd\t", 4) == 0 ||
             strncmp(buf, "CWD\n", 4) == 0 || strncmp(buf, "cwd\n", 4) == 0)
    {

        buf = buf + 4;
        strtok(buf, "\n");
        if (chdir(buf) == -1)
        {
            if (errno == EACCES)
            {
                strcpy(buf, "339 Permission denied");
                send(sock, buf, MAXLINE, 0);
                return 0;
            }

            else if (errno == ENOTDIR || errno == ENOENT)
            {
                strcpy(buf, "340 Invalid Derectory");
                send(sock, buf, MAXLINE, 0);
                return 0;
            }

            else
            {
                strcpy(buf, "341 couldnot change directory.");
                send(sock, buf, MAXLINE, 0);
                return 0;
            }
        }
        // printf("CWD called \n");
        strcpy(buf, "Working direcory changed Successfully");
        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    // Implementation for removing a specified directory on the server file system
    else if (strncmp(buf, "RMD ", 4) == 0 || strncmp(buf, "rmd ", 4) == 0 ||
             strncmp(buf, "RMD\t", 4) == 0 || strncmp(buf, "rmd\t", 4) == 0 ||
             strncmp(buf, "RMD\n", 4) == 0 || strncmp(buf, "rmd\n", 4) == 0)
    {

        buf = buf + 5;
        // remove_directory(buf);
        // printf("RMD called \n");
        strcpy(buf, "Not Implemented");
        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    // List of all files & folders on the server file system for current working directory
    else if (strncmp(buf, "LIST ", 5) == 0 || strncmp(buf, "list ", 5) == 0 ||
             strncmp(buf, "LIST\t", 5) == 0 || strncmp(buf, "list\t", 5) == 0 ||
             strncmp(buf, "LIST\n", 5) == 0 || strncmp(buf, "list\n", 5) == 0)
    {

        buf = buf + 5;
        printf("%s\n", buf);
        DIR *directory;
        struct dirent *entry;
        *list = '\0';
        char *str[240];

        if (strlen(buf) > 1)
        {
            strtok(buf, "\n");
            if ((directory = opendir(buf)) == NULL)
            {
                printf("346 %s \n", buf);
            }
        }

        else
        {
            if ((directory = opendir("./")) == NULL)
            {
                printf("3462 error%s \n", buf);
            }
        }
        // Loop to gather all the directories & subdirectories 
        while ((entry = readdir(directory)) != NULL)
        {
            strcat(list, "-> ");
            strcat(list, entry->d_name);
            strcat(list, "\n");
        }
        // Closing the directory safely 
        closedir(directory);
        send(sock, list, MAXLINE, 0);
        return 0;
    }

    // Create a directory on the server file system with custom name and path
    else if (strncmp(buf, "MKD ", 4) == 0 || strncmp(buf, "mkd ", 4) == 0 ||
             strncmp(buf, "MKD\t", 4) == 0 || strncmp(buf, "mkd\t", 4) == 0 ||
             strncmp(buf, "MKD\n", 4) == 0 || strncmp(buf, "mkd\n", 4) == 0)
    {

        buf = buf + 4;
        strtok(buf, "\n"); // to remove trailing '\n'
        int status = stat(buf, &st);
        if (status == -1)
        {
            mkdir(buf, 0700);
            strcpy(buf, "Directory created successfully");
            send(sock, buf, MAXLINE, 0);
            return 0;
        }

        if (status == 0)
        {
            strcpy(buf, "Directory already exist");
            send(sock, buf, MAXLINE, 0);
            return 0;
        }

        strcpy(buf, "Directory can't be created");
        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    // Safely quit the ftp server 
    else if (strncmp(buf, "QUIT ", 5) == 0 || strncmp(buf, "quit ", 5) == 0 ||
             strncmp(buf, "QUIT\t", 5) == 0 || strncmp(buf, "quit\t", 5) == 0 ||
             strncmp(buf, "QUIT\n", 5) == 0 || strncmp(buf, "quit\n", 5) == 0)
    {

        buf = buf + 4;
        strcpy(buf, "Bye Bye \n");
        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    // Implementation for ABORT -> Yet to be implemented completely.
    else if (strncmp(buf, "ABOR ", 5) == 0 || strncmp(buf, "abor ", 5) == 0 ||
             strncmp(buf, "ABOR\t", 5) == 0 || strncmp(buf, "abor\t", 5) == 0 ||
             strncmp(buf, "ABOR\n", 5) == 0 || strncmp(buf, "abor\n", 5) == 0)
    {

        buf = buf + 4;
        strcpy(buf, "ABOR");
        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    // Delete a file on the server file system.
    else if (strncmp(buf, "DELE ", 5) == 0 || strncmp(buf, "dele ", 5) == 0 ||
             strncmp(buf, "DELE\t", 5) == 0 || strncmp(buf, "dele\t", 5) == 0 ||
             strncmp(buf, "DELE\n", 5) == 0 || strncmp(buf, "dele\n", 5) == 0)
    {

        buf = buf + 5;
        strtok(buf, "\n");
        if (remove(buf) == 0)
        {
            strcpy(buf, "The file is deleted successfully.");
        }
        else
        {
            strcpy(buf, "The file is not deleted.");
        }

        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    // change the current working directory to the parent dir of specified path.
    else if (strncmp(buf, "CDUP ", 5) == 0 || strncmp(buf, "cdup ", 5) == 0 ||
             strncmp(buf, "CDUP\t", 5) == 0 || strncmp(buf, "cdup\t", 5) == 0 ||
             strncmp(buf, "CDUP\n", 5) == 0 || strncmp(buf, "cdup\n", 5) == 0)
    {

        buf = buf + 4;
        strcpy(buf, dirname(buf));

        if (chdir(buf + 1) == -1)
        {
            strcpy(buf, "error in change directory");
        }
        else
        {
            strcpy(buf, "Working directory change successfully");
        }
        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    // Sends ok message to client to implement continue feature 
    else if (strncmp(buf, "NOOP ", 5) == 0 || strncmp(buf, "noop ", 5) == 0 ||
             strncmp(buf, "NOOP\t", 5) == 0 || strncmp(buf, "noop\t", 5) == 0 ||
             strncmp(buf, "NOOP\n", 5) == 0 || strncmp(buf, "noop\n", 5) == 0)
    {
        strcpy(buf, "OK");
        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    // Store file located on the client machine to server with proper authorization and content.
    // Upload client's file to server file system.
    else if (strncmp(buf, "STOR ", 5) == 0 || strncmp(buf, "stor ", 5) == 0 ||
             strncmp(buf, "STOR\t", 5) == 0 || strncmp(buf, "stor\t", 5) == 0 ||
             strncmp(buf, "STOR\n", 5) == 0 || strncmp(buf, "stor\n", 5) == 0)
    {
        buf = buf + 5;
        printf("%s \n", buf);
        store_file(buf, sock);
        return 0;
    }

    // Retrieve file located on the server machine to client with proper authorization and content.
    // Download server's file to client's file system.
    else if (strncmp(buf, "RETR ", 5) == 0 || strncmp(buf, "retr ", 5) == 0 ||
             strncmp(buf, "RETR\t", 5) == 0 || strncmp(buf, "retr\t", 5) == 0 ||
             strncmp(buf, "RETR\n", 5) == 0 || strncmp(buf, "retr\n", 5) == 0)
    {   
        // send(sock, buf, MAXLINE, 0);
        buf=buf + 5;
        printf("%s \n", buf);
        send_file(buf, sock);
        command = RETR;
        return 0;
    }

    else if (strncmp(buf, "PORT ", 5) == 0 || strncmp(buf, "port ", 5) == 0 ||
             strncmp(buf, "PORT\t", 5) == 0 || strncmp(buf, "port\t", 5) == 0 ||
             strncmp(buf, "PORT\n", 5) == 0 || strncmp(buf, "port\n", 5) == 0)
    {
        strcpy(buf, "NOT IMPELEMENTED");
        send(sock, buf, MAXLINE, 0);
        return 0;
    }
    // Invalid command entered
    else
    {
        strcpy(buf, "Invalid Command Entered");
        send(sock, buf, MAXLINE, 0);
        return 0;
    }

    while (*buf == ' ' || *buf == '\t')
    {
        buf++;
    }

    return 0;
}