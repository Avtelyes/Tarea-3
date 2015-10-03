//
//  main.c
//  Ejercicio3
//
//  Created by Josué Carlos García Puig on 30/09/15.
//  Copyright © 2015 Avtelyes. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

int calculaFact(int);

int main(void)
{
    union semun {
        int val;                  /* valor para SETVAL */
        struct semid_ds *buf;     /* buffer para IPC_STAT, IPC_SET */
        unsigned short *array;    /* arreglo para for GETALL, SETALL */
        /* Linux specific part: */
        struct seminfo *__buf;    /* buffer para IPC_INFO */
    };
    
    int pid;
    int semid; /* id del semaforo */
    key_t key = 474; /* llave para semget() */
    int semflg = IPC_CREAT | 0666; /* semflg para semget() */
    int nsems = 1; /* nsems para semget() */
    int rc = 0;
    int i, j;
    union semun sem_union;
    sem_union.val = 1;
    
    
    // Inicializacion del semaforo
    if ((semid = semget(key, 2, semflg)) == -1)
    {
        perror("semget: semget failed");
        exit(1);
    } else {
        printf("created successfully: id =%d\n", semid);
    }
    
    if ( semctl(semid, 0, SETVAL, sem_union) == -1)
    {
        fprintf(stderr, "(#%d) ", errno);
        perror("semctl() failed");
        exit(EXIT_FAILURE);
    }
    
    // Fork
    if ((pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }
    
    // Estados Reader/Writer
    if (pid == 0)
    {
        printf("%d",semctl(semid, nsems, GETVAL, sem_union));
        while (semctl(semid,nsems,GETVAL,sem_union) != -1)
        {
            //printf("Proceso Hijo");
            if (semctl(semid,0,GETVAL,sem_union) == 1){
                int n;
                //sleep(2);
                n = semctl(semid,nsems,GETVAL,sem_union);
                printf("El factorial de %d es: %d\n",n,calculaFact(n));
                sem_union.val = 0;
                semctl(semid,0,SETVAL,sem_union);
            }
        }
    } else {
        while (1)
        {
            printf("Dame un numero: ");
            sleep(1);
            scanf(" %d", &sem_union.val);
            semctl(semid, 1, SETVAL, sem_union);
            if(sem_union.val == 0)
                break;
            sem_union.val=1;
            semctl(semid, 0, SETVAL, sem_union);
            
        }
        
        semctl(semid, 0, IPC_RMID, sem_union);
        exit(0);
    }
}

int calculaFact(int num)
{
    int c = 0, factorial = 1;
    for (c = 1; c <= num; c++)
        factorial = factorial * c;
    
    return factorial;
}