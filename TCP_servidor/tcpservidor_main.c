/*
 * XXXXXX.c
 * 
 * Copyright 2019 INFIN (EUSS) <euss@euss.cat>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

/***************************************************************************
                          main.c  -  server
                             -------------------
    begin                : lun feb  4 15:30:41 CET 2002
    copyright            : (C) 2002 by A. Moreno
    copyright            : (C) 2020 by A. Fontquerni
    email                : amoreno@euss.es
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define SERVER_PORT_NUM		5001
#define SERVER_MAX_CONNECTIONS	4

#define REQUEST_MSG_SIZE	1024


//VARIABLES GLOBALES

char	buffer[256];
char	missatge[300];
struct sockaddr_in	serverAddr;
struct sockaddr_in	clientAddr;
unsigned int	sockAddrSize;
int			sFd;
int			newFd;
int 		result;

//FUNCION DE MANIPULACION DE DATOS 

void manipulacion{
            
            if (strcmp(buffer,"{U}") == 0) {                
                muestra_antigua();
                enviar();
            } 
            else if (strcmp(buffer, "{X}") == 0 ){
                muestra_maxima();
                enviar();
            }
            else if (strcmp(buffer, "{Y}") == 0 ){
                muestra_minima();
                enviar();
            }
            else if (strcmp(buffer, "{R}") == 0 ){
                reset_max_min();
                enviar();
            }
            else if (strcmp(buffer, "{B}") == 0 ){
                numero_muestras_array();
                enviar();
            }
            else if (buffer[2] == 0 ){
                paro();
                enviar();
            }
            else if (buffer[2] == 1 ){
                marcha();
                enviar();
            }
            else sprintf(buffer,"Mensaje no corresponde a nada")
   }

//FUNCION MUESTRA ANTIGUA

void muestra_antigua(){
    
    }
    
//FUNCION MUESTRA MAXIMA

void muestra_maxima(){
    
    }
    
//FUNCION MUESTRA MINIMA

void muestra_minima(){
    
    }
    
//FUNCION RESET MAXIMO Y MINIMO

void reset_max_min(){
    
    }
    
//FUNCION NUMERO MUESTRAS

void numero_muestras_array(){
    
    }

//FUNCION PARO

void paro(){
    
    }

//FUNCION MARCHA

void marcha(){
    
    }
    
//FUNCION ENVIAR

void enviar(){
        /*Enviar*/
		strcpy(buffer,missatge); //Copiar missatge a buffer
		result = write(newFd, buffer, strlen(buffer)+1); //+1 per enviar el 0 final de cadena
		printf("Missatge enviat a client(bytes %d): %s\n",	result, missatge);
}

/************************
*
*
* tcpServidor
*
*
*/

int main(int argc, char *argv[]) {
    
	/*Preparar l'adreça local*/
	sockAddrSize=sizeof(struct sockaddr_in);
	bzero ((char *)&serverAddr, sockAddrSize); //Posar l'estructura a zero
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*Crear un socket*/
	sFd=socket(AF_INET, SOCK_STREAM, 0);
	
	/*Nominalitzar el socket*/
	result = bind(sFd, (struct sockaddr *) &serverAddr, sockAddrSize);
	
	/*Crear una cua per les peticions de connexió*/
	result = listen(sFd, SERVER_MAX_CONNECTIONS);
	
	/*Bucle s'acceptació de connexions*/

	while(1){

		printf("\nServidor esperant connexions\n");

		/*Esperar conexió. sFd: socket pare, newFd: socket fill*/
		newFd=accept(sFd, (struct sockaddr *) &clientAddr, &sockAddrSize);
		printf("Connexión acceptada del client: adreça %s, port %d\n",	inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		/*Rebre*/
		memset( buffer, 0, 256 );
		result = read(newFd, buffer, 256);
		printf("Missatge rebut del client(bytes %d): %s\n",	result, buffer);
        
        manipulacion();
		/*Tancar el socket fill*/
		result = close(newFd);
	}
}




