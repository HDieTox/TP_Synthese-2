#include <stdlib.h>
#include <stdio.h>

void slicer(char* sliced[],char* string, char separator){
    
    char* strToken = strtok(string,&separator);
    int i = 0;

    while (strToken != NULL)
    {
        sliced[i] = strToken;
        strToken = strtok(NULL,&separator);
        i++;
    }
}