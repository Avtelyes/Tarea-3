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

int factorial(int);

int main(void)
{
    union semun {
        int val;                  /* value for SETVAL */
        struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
        unsigned short *array;    /* array for GETALL, SETALL */
        /* Linux specific part: */
        struct seminfo *__buf;    /* buffer for IPC_INFO */
    };
    
    int pid;
    int semid; /* semid of semaphore set */
    key_t key = 474; /* key to pass to semget() */
    int semflg = IPC_CREAT | 0666; /* semflg to pass to semget() */
    int nsems = 1; /* nsems to pass to semget() */
    int rc = 0;
    int i, j;
    union semun sem_union;
    sem_union.val = 1;
    
    
    // Initiate mutex semaphore
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
    
    // Start Reader/Writer
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
                printf("El factorial de %d es: %d\n",n,factorial(n));
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


int factorial(int n){
    
    int res = 1;
    int i ;
    for (i=1;i<=n;i++){
        res *= i;
    }
    return res;
    
}
