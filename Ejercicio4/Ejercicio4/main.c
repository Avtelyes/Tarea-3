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

/*DEfinicion de una estructura para el mensaje*/
struct my_msg{
    long msgtype;
    int value;
};

int calculaFact(int);

int main(int argc, const char * argv[]) {
    
    pid_t hijo;
    
    hijo = fork();
    
    /*Llave para msgget()*/
    key_t key;
    /*msgflg para msgget()*/
    int msgflg;
    /*id de msgget()*/
    int msgid;
    /*instancia de la estructura*/
    struct my_msg sometext;
    /*Inicializacion de la llave*/
    key = 1234;
    
    msgflg = 0666 | IPC_CREAT;
    /*Creacion de la cola */
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
            printf("El factorial es: %d\n", calculaFact(sometext.value));
            if(sometext.value == 0){
                printf("message ends.\n");
                /*delete the message queue */
                if(msgctl(msgid, IPC_RMID, 0) == -1){
                    perror("msgctl: msgctl failed");
                    exit(1);
                }
                printf("Programa terminado\n");
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
            sleep(1);
            printf("Dame un numero:\n");
            //fgets(buffer, BUFSIZ, stdin);
            scanf("%d", &buffer);
            /*Tipo de mensaje */
            sometext.msgtype = 1;
            /*Copiar del buffer*/
            //strcpy(sometext.message, buffer);
            sometext.value = buffer;
            /*Envio del mensaje */
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

int calculaFact(int num)
{
    int c = 0, factorial = 1;
    for (c = 1; c <= num; c++)
        factorial = factorial * c;
    
    return factorial;
}

