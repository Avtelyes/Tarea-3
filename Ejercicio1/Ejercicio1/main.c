//
//  main.c
//  Ejercicio1
//
//  Created by Josué Carlos García Puig on 29/09/15.
//  Copyright © 2015 Avtelyes. All rights reserved.
//

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


void readFile();

int main(int argc, char **argv) {
    
    char *arg_values = NULL;
    int c = 0;
    pid_t hijo, sid;
    
    while ((c = getopt(argc, argv, "m:")) != -1) {
        switch (c) {
            case 'm':
                arg_values = optarg;
                break;
            case '?':
                return 1;
            default:
                abort();
        }
    }
    
    int min = atoi(arg_values);
    
    
    hijo = fork();
    if (hijo < 0) {
        exit(EXIT_FAILURE);
    }
    if (hijo > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    if ((chdir("/")) < 0) {
       	exit(EXIT_FAILURE);
    }
    
    while (1){
        readFile();
        printf("\n");
        sleep(60*min);
    }
    
    //exit(0);
    
    return 0;
}

void readFile(){
    struct dirent *dir;
    DIR *pDir;
    
    pDir = opendir ("/proc");
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n","/proc");
        return;
    }
    
    printf ("PID    NAME      STATUS\n");
    while ((dir = readdir(pDir)) != NULL) {
        if (atoi(dir->d_name) != 0) {
            char *path = (char *)malloc(100*sizeof(char));
            strcpy(path,"/proc/");
            strcat(path,dir->d_name);
            strcat(path,"/status");
            FILE *fr;
            fr = fopen (path,"r");
            char *info = (char*)malloc(100*sizeof(char));
            fgets(info,100,fr);
            size_t length = strlen(info);
            if (*(info+length-1) == '\n')
                *(info+length-1) = '\0';
            char *info2 = (char*)malloc(100*sizeof(char));
            fgets(info2,100,fr);
            printf("%d    %s     %s",atoi(dir->d_name),info+5,info2+7);
            
            free(info);
            free(info2);
            free(path);
            fclose(fr);
        }
    }
    
    closedir (pDir);
}
