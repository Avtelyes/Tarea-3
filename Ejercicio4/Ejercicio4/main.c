//
//  main.c
//  Ejercicio4
//
//  Created by Josué Carlos García Puig on 01/10/15.
//  Copyright © 2015 Avtelyes. All rights reserved.
//

#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TEXT 5

/*define a message structure*/
struct my_msg{
    long msgtype;
    int value;
};

int main(int argc, const char * argv[]) {
    
    pid_t hijo;
    
    hijo = fork();
    
    /*key to be passed to msgget()*/
    key_t key;
    /*msgflg to be passed to msgget()*/
    int msgflg;
    /*return value from msgget()*/
    int msgid;
    /*declare an instance of structure of msg*/
    struct my_msg sometext;
    /*initialize the key*/
    key = 1234;
    /* read and write permission for owner,
     and create a message queue if not exists*/
    msgflg = 0666 | IPC_CREAT;
    /*creat a message queue */
    if((msgid = msgget(key, msgflg))== 1){
        perror("msgget: msgget failed");
        exit(1);
    }
    printf("msgget succeeded\n");
    
    if(hijo < 0)
    {
        perror("Hubo un error al crear el proceso\n");
        exit(-1);
    }
    else if (hijo == 0)
    {
        long int msgpriori = 0;
        while(1){
            /*receive the message */
            if (msgrcv(msgid, &sometext, BUFSIZ, msgpriori, 0) == -1){
                perror("msgrcv: msgrcv failed\n");
                exit(1);
            }
            printf("You wrote: %d\n", sometext.value);
            if(sometext.value == 0){
                printf("message ends.\n");
                /*delete the message queue */
                if(msgctl(msgid, IPC_RMID, 0) == -1){
                    perror("msgctl: msgctl failed");
                    exit(1);
                }
                printf("program succeeded\n");
                break;
            }
        }
        exit(0);
        
    }
    else
    {
        while(1)
        {
            int buffer;
            /*read user input from the keyboard */
            printf("Please ener some text:\n");
            //fgets(buffer, BUFSIZ, stdin);
            scanf("%d", &buffer);
            //printf("Introdujiste: %s\n", buffer);
            /*set the msgtype */
            sometext.msgtype = 1;
            /*copy from buffer to message*/
            //strcpy(sometext.message, buffer);
            sometext.value = buffer;
            /*send the message */
            if (msgsnd(msgid, &sometext, MAX_TEXT, 0) == -1){
                perror("msgsnd: msgsnd failed\n");
                exit(1);
            }
            if(buffer == 0){
                printf("message ends.\n");
                break;
            }
        }
        
        exit(0);
        
    }
    
    return 0;
}
