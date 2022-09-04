/*
 Mehul & Rahil
 Implemetation for custom split function used in various scenerios such as extracting file name, path of a file, etc.
*/
#include "header.h"

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
