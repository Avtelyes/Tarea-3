//
//  main.c
//  Ejercicio2
//
//  Created by Josué Carlos García Puig on 29/09/15.
//  Copyright © 2015 Avtelyes. All rights reserved.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h> /* shm*  */

#define FILEKEY "/bin/cat"
#define KEY 1300
#define MAXBUF 10

int main(int argc, const char * argv[]) {
    
    pid_t hijo;
    
    int fd[2];
    pipe(fd);
    
    //int numero = 1, suma = 0;
    
    hijo = fork();
    
    
    /* Se define la llave para crear el identificador IPC con un numero que conozcan ambos procesos */
    int key = ftok(FILEKEY, KEY);
    if (key == -1) {
        fprintf (stderr, "Error con la llave \n");
        return -1;
    }
    
    /* Se crea la memoria compartida */
    int id_zone = shmget (key, sizeof(int)*MAXBUF, 0777 | IPC_CREAT);
    if (id_zone == -1) {
        fprintf (stderr, "Error al crear la zona de memoria \n");
        return -1;
    }
    
    printf ("ID de la memoria compartida: %i\n", id_zone);
    
    int *buffer; /* buffer compartido */
    /* Se reserva la memoria a compartir */
    buffer = shmat (id_zone, (char *)0, 0);
    if (buffer == NULL) {
        fprintf (stderr, "Error al reservar la memoria compartida \n");
        return -1;
    }
    
    printf ("Apuntador a la memoria compartida: %p\n", buffer);
    
    
    if(hijo < 0)
    {
        perror("Hubo un error al crear el proceso\n");
        exit(-1);
    }
    else if (hijo == 0)
    {
        int num = 1;
        *buffer = 99;
        /*for (i = 0; i < MAXBUF; i++)
         buffer[i] = i;*/
        while(*buffer != 0)
        {
            //sleep(1);
            printf("Dame un numero: ");
            scanf("%d",&num);
            sleep(1);
            *(buffer + 1) = num;
            
            
            if(num != 0)
                *buffer = 99;
            else
                *buffer = 0;
        }
        
        /* The daemon executes until press some character */
        /*char c;
         c = getchar();*/
        exit(0);
        
    }
    else
    {
        //wait(NULL);
        /* Write the values of shared memory */
        while(1)
        {
            if(*buffer == 99 )
            {
                int num;
                num = *(buffer+1);
                int c = 0, factorial = 1;
                for (c = 1; c <= num; c++)
                    factorial = factorial * c;
                //sleep(1);
                printf("El factorial es de: %d\n", factorial);
                *buffer = 1;
            }
            else if (*buffer == 0)
                break;
        }
        
        //printf("%d",buffer[0]);
        
        
        /* Free the shared memory */
        shmdt ((char *)buffer);
        shmctl (id_zone, IPC_RMID, (struct shmid_ds *)NULL);
        exit(0);
        
    }
    
    
    return 0;
}

