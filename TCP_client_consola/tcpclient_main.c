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

char 		missatge[200];
int			sFd;
int			mlen;
int 		result;
char		buffer[256];
struct sockaddr_in	serverAddr;
char	    serverName[] = "127.0.0.1"; //Adreça IP on est� el servidor
int			sockAddrSize;

//PROTOTIPOS DE FUNCIONES

void FuncionamientoMenu();
void opciones();
void muestra_antigua();
void muestra_maxima();
void muestra_minima();
void reset_max_min();
void numero_muestras_array();
void marcha_paro();
void recibir();
void conexion();

//FUNCION CONEXION

void conexion(){

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
}

//FUNCION RECIBIR

void recibir(){
             /*Rebre*/
			result = read(sFd, buffer, 256);
			printf("\nMissatge rebut del servidor(bytes %d): %s\n",	result, buffer);
    }

//FUNCION DE MARCHA PARO

void marcha_paro(){
	
	int v;
	int t1;
	int nmediana;

	printf("Seleccione el modo que desea: 1 (marcha) o 0 (paro)\n");
	scanf("%d",&v);		
				
			if(v==1){
				
				printf("Has puesto el modo marcha porfavor ingrese el tiempo de muestreo que desea en segundos: \n");
				scanf("%d",&t1);
				printf("Ingrese el numero de muestras de las que desea hacer la mediana: \n");
				scanf("%d",&nmediana);
				printf("Se hara el muestreo de %d segundos y se hara la media con %d muestras\n",t1,nmediana);
				sprintf(missatge, "{M %d %d %d}\n",v,t1,nmediana);
			}

			else{				
				
				printf("Ha puesto el modo paro\n");
				sprintf(missatge, "{M 0 0 0}\n");
			}

				/*Enviar*/
				strcpy(buffer,missatge); //Copiar missatge a buffer
				result = write(sFd, buffer, strlen(buffer));
				printf("\nMissatge enviat a servidor(bytes %d): %s\n",	result, missatge);
	}


//FUNCION QUE MUESTRAN EL NUMERO DE MUESTRAS EN EL ARRAY

void numero_muestras_array(){
				
				printf("Has pedido el numero de muestras\n");	
				sprintf(missatge, "{B}\n");
							
				/*Enviar*/
				strcpy(buffer,missatge); //Copiar missatge a buffer
				result = write(sFd, buffer, strlen(buffer));
				printf("\nMissatge enviat a servidor(bytes %d): %s\n",	result, missatge);
	}

//FUNCION DE RESETEO DEL MAXIMO Y DEL MINIMO

void reset_max_min(){
				
				printf("Has reseteado el valor maximo y minimo\n");	
				sprintf(missatge, "{R}\n");
	}

//FUNCION DE MUESTRA MINIMA

void muestra_minima(){
	
				printf("Has pedido la muestra minima\n");	
				sprintf(missatge, "{Y}\n");
							
				/*Enviar*/
				strcpy(buffer,missatge); //Copiar missatge a buffer
				result = write(sFd, buffer, strlen(buffer));
				printf("\nMissatge enviat a servidor(bytes %d): %s\n",	result, missatge);
	}

//FUNCION DE MUESTRA MAXIMA

void muestra_maxima(){

				printf("Has pedido la muestra maxima\n");	
				sprintf(missatge, "{X}\n");
				
				/*Enviar*/
				strcpy(buffer,missatge); //Copiar missatge a buffer
				result = write(sFd, buffer, strlen(buffer));
				printf("\nMissatge enviat a servidor(bytes %d): %s\n",	result, missatge);
	}

//FUNCION DE MUESTRA ANTIGUA

void muestra_antigua(){
				
				printf("Has pedido la muestra mas antigua\n");
				sprintf(missatge, "{U}\n");
				
				/*Enviar*/
				strcpy(buffer,missatge); //Copiar missatge a buffer
				result = write(sFd, buffer, strlen(buffer));
				printf("\nMissatge enviat a servidor(bytes %d): %s\n",	result, missatge);
	}
	
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
	printf("s: Para salir del programa\n");
	printf("--------------------\n");
	}


//FUNCION DE COMO FUNICONA EL MENU

void FuncionamientoMenu()
{
	char input;
	
	opciones();		//muestra el menu
	
	input = getchar();         //le pedimos un valor al usuario

	while(input!= 's'){
		
		switch (input)
		{
			case '1':
				conexion();
                muestra_antigua();
				recibir();
                opciones();
                break;
			
			case '2':
				conexion();
                muestra_maxima();
				recibir();
                opciones();
                break;
			
			case '3':
				conexion();
                muestra_minima();
				recibir();
                opciones();
                break;
			
			case '4':
				conexion();
                reset_max_min();
				recibir();
                opciones();
                break;
			
			case '5':
				conexion();
                numero_muestras_array();
				recibir();
                opciones();
                break;
			
			case '6':
                conexion();
				marcha_paro();
                recibir();
                opciones();
                break;

			case 0x0a: //Això és per enviar els 0x0a (line feed) que s'envia quan li donem al Enter
				break;
		
			default:
				printf("Opció incorrecta\n");	
				printf("He llegit 0x%hhx \n",input);
				opciones();
				break;
		}

	input = getchar();

	}
			/*Tancar el socket*/
				close(sFd);
				
}

/************************
*
*
* tcpClient
*
*
*/

int main(int argc, char *argv[]){

	FuncionamientoMenu(); 
	
	return 0;
}