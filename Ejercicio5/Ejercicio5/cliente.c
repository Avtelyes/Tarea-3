//
//  cliente.c
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
#include <sys/un.h>

#define TCP_PORT 8000

int main(int argc, const char * argv[]) {
    
    struct sockaddr_un direccion;
    char buffer[1000];
    
    int cliente;
    
    int leidos, escritos;
    
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }
    
    //Crear el socket
    cliente = socket(PF_LOCAL, SOCK_STREAM, 0);
    const char* const client_socket = argv[1];
    
    // Establecer conexión
    //direccion.sin_port = htons(TCP_PORT);
    direccion.sun_family = AF_LOCAL;
    //inet_aton(argv[1], &direccion.sin_addr);
    strcpy(direccion.sun_path,client_socket);
    
    int estado = connect(cliente, (struct sockaddr *) &direccion, SUN_LEN(&direccion));
    
    // Comunicación
    
    if (estado == 0) {
        printf("Conectado \nDame un numero para calcular factorial:\n");
        
        // Leer de teclado y escribir en socket
        while (leidos = read(fileno(stdin), &buffer, sizeof(buffer))) {
            write(cliente, &buffer, leidos);
            
            leidos = read(cliente, &buffer, sizeof(buffer));
            write(fileno(stdout), &buffer, leidos);
        }
    }
    
    // Cerrar el socket
    close(cliente);
    
    return 0;
}

