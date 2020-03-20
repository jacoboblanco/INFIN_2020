/***************************************************************************
                          main.c  -  client
                             -------------------
    begin                : lun feb  4 16:00:04 CET 2002
    copyright            : (C) 2002 by A. Moreno
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


#define REQUEST_MSG_SIZE	1024
#define REPLY_MSG_SIZE		500
#define SERVER_PORT_NUM		5001

//VARIABLES GLOBALES

char missatge[200];

//PROTOTIPOS DE FUNCIONES

void ImprimirMenu(void);
void opciones();

//FUNCION DE OPCIONES EN EL MENU

void opciones(){

	printf("\n\nMenu:\n");
	printf("--------------------\n");
	printf("1: Muestra nas antigua\n");
	printf("2: Muestra maxima\n");
	printf("3: Muestra minima\n");
	printf("4: Reset de muestra maxima y minima\n");
	printf("5: Numero de muestras\n");
	printf("6: Marcha con tiempo de muestreo y mediana de valor de las muestras\n");
	printf("--------------------\n");

	}


//FUNCION DE MOSTRAR EL MENU

void ImprimirMenu()
{
	char input;
	int v;
	int t1;
	int nmediana;
	
	opciones();
	
	input = getchar();

		switch (input)
		{
			case '1':
				printf("Has pedido la muestra mas antigua\n");
				sprintf(missatge, "{U}\n");
				opciones();
				break;
			
			case '2':
				printf("Has pedido la muestra maxima\n");	
				sprintf(missatge, "{X}\n");
				opciones();
				break;
			
			case '3':
				printf("Has pedido la muestra minima\n");	
				sprintf(missatge, "{Y}\n");
				opciones();
				break;
			
			case '4':
				printf("Has reseteado el valor maximo y minimo\n");	
				sprintf(missatge, "{R}\n");
				opciones();
				break;
			
			case '5':
				printf("Has pedido el numero de muestras\n");	
				sprintf(missatge, "{B}\n");
				opciones();
				break;
			
			case '6':
				
				printf("Seleccione el modo que desea: 1 (marcha) o 2 (paro)\n");
				scanf("%d",&v);
				if(v==1){
					printf("Has puesto el modo marcha porfavor ingrese el tiempo de muestreo que desea en segundos: \n");
					scanf("%d",&t1);
					printf("Ingrese el numero de muestras de las que desea hacer la mediana: \n");
					scanf("%d",&nmediana);
					printf("Se hara el muestreo de %d segundos y se hara la media con %d muestras\n",t1,nmediana);
					sprintf(missatge, "{M %d %d %d}\n",v,t1, nmediana);
					opciones();
					break;
				}
				else {
					printf("Ha puesto el modo paro");
					sprintf(missatge, "{M 0 0 0}\n");
					opciones();
					break;
					}
				
				
			case 0x0a: //Això és per enviar els 0x0a (line feed) que s'envia quan li donem al Enter
				break;
			
			default:
				printf("Opció incorrecta\n");	
				printf("He llegit 0x%hhx \n",input);
				opciones();
				break;
		}

	}
	

 /************************
*
*
* tcpClient
*
*
*/
int main(int argc, char *argv[]){
	struct sockaddr_in	serverAddr;
	char	    serverName[] = "127.0.0.1"; //Adreça IP on est� el servidor
	int			sockAddrSize;
	int			sFd;
	int			mlen;
	int 		result;
	char		buffer[256];
	
	/*Crear el socket*/
	sFd=socket(AF_INET,SOCK_STREAM,0);

	/*Construir l'adreça*/
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero ((char *)&serverAddr, sockAddrSize); //Posar l'estructura a zero
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons (SERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = inet_addr(serverName);

	/*Conexió*/
	result = connect (sFd, (struct sockaddr *) &serverAddr, sockAddrSize);
	if (result < 0)
	{
		printf("Error en establir la connexió\n");
		exit(-1);
	}
	printf("\nConnexió establerta amb el servidor: adreça %s, port %d\n",	inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

	ImprimirMenu();
	
	/*Enviar*/
	strcpy(buffer,missatge); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, missatge);

	/*Rebre*/
	result = read(sFd, buffer, 256);
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);

	/*Tancar el socket*/
	close(sFd);

	return 0;
	}
	
