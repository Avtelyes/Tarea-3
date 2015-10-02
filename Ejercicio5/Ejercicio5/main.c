//
//  main.c
//  Ejercicio5
//
//  Created by Josué Carlos García Puig on 01/10/15.
//  Copyright © 2015 Avtelyes. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>


#define TCP_PORT 8000

int main(int argc, const char * argv[]) {
    
    struct sockaddr_un direccion;
    char buffer[1000];
    
    int servidor, cliente;
    
    int leidos, numero1;
    
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }
    
    //Crear el socket
    servidor = socket(PF_LOCAL, SOCK_STREAM, 0);
    const char* const server_socket = argv[1];
    
    // Enlace con el socket
    //direccion.sin_port = htons(TCP_PORT);
    direccion.sun_family = AF_LOCAL;
    strcpy(direccion.sun_path,server_socket);
    //inet_aton(argv[1], &direccion.sin_addr);
    
    bind(servidor, &direccion, SUN_LEN(&direccion));
    
    // Escuchar a los clientes
    listen(servidor, 10);
    
    // Comunicación
    socklen_t tamano = sizeof(direccion);
    
    cliente = accept(servidor, (struct sockaddr *) &direccion, &tamano);
    
    if (cliente >= 0) {
        printf("Aceptando conexiones \n");
        
        // Leer de socket y escribir en pantalla
        while (leidos = read(cliente, &buffer, sizeof(buffer))) {
            write(fileno(stdout), &buffer, leidos);
            numero1 = atoi(buffer);
            if(numero1 == 0)
                break;
            int factorial = 1, c = 0;
            for (c = 1; c <= numero1; c++)
                factorial = factorial * c;
            sprintf(buffer, "%s%d%s", "El factorial es: ", factorial, "\n");
            //strcpy(buffer, &"El numero es: "[numero1]);
            int bytes = strlen(buffer);
            //leidos = read(fileno(stdin), &buffer, sizeof(buffer));
            write(cliente, &buffer, bytes);
        }
        
        
    }
    
    // Cerrar el socket
    
    close(servidor);
    close(cliente);
    
    return 0;
}
