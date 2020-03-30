
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
#include <time.h>

#define SERVER_PORT_NUM		5001
#define SERVER_MAX_CONNECTIONS	4

#define REQUEST_MSG_SIZE	1024

#define t_max 100


//VARIABLES GLOBALES

char	buffer[256];
char	missatge[200];
struct sockaddr_in	serverAddr;
struct sockaddr_in	clientAddr;
unsigned int	sockAddrSize;
int			sFd;
int			newFd;
int 		result;
int i=0;
int j=0;
int unsigned frente=0;
int unsigned n=0;
float *muestra;
float datos[t_max];

//PROTOTIPOS DE FUNCIONES

void manipulacion();
void muestra_antigua();
void muestra_maxima();
void muestra_minima();
void reset_max_min();
void numero_muestras_array();
void paro();
void marcha();
void enviar();
void llenar_array();
void adquirir_muestra(int N);
void cola_circular(float *muestra, int x);

//FUNCION MUESTRA ANTIGUA

void muestra_antigua(){

        float ultimo;
        ultimo = datos[j];
        sprintf(missatge, "{U 0 %.2f}",ultimo);
        frente=(frente+1)%t_max;
        n--;
   }

//FUNCION RESET MAXIMO Y MINIMO

void reset_max_min(){
        
        float menor, mayor;
        float *p_menor, *p_mayor;
        
        mayor = datos[0]; 
        menor = datos[0];
        
        for (int i=0;i<t_max;i++){
            if (datos[i]>mayor){
            p_mayor=&datos[i];
            }
        }
        
        for (int i=0;i<t_max;i++){
            if (datos[i]<menor){
			p_menor=&datos[i];
            }
        }
        
        p_menor = 0;    
        p_mayor = 0;
        
        sprintf(missatge,"{R 0}");
  }   

//FUNCION NUMERO MUESTRAS

void numero_muestras_array(){

        sprintf(missatge, "{B 0 %d}",j);
 
 }

//FUNCION MUESTRA MAXIMA

void muestra_maxima(){
        
        float mayor;
        mayor = datos[0]; 
        
        for (int i=0;i<t_max;i++){
            if (datos[i]>mayor){
            mayor=datos[i];
            }
        }

        sprintf(missatge, "{X 0 %.2f}",mayor);
        
    }
    
//FUNCION MUESTRA MINIMA

void muestra_minima(){
            
        float menor;
        menor = datos[0];
    
        for (int i=0;i<t_max;i++){
            if (datos[i]<menor){
			menor=datos[i];
            }
        }
        
        sprintf(missatge, "{Y 0 %.2f}",menor);
        
    }
 
//FUNCION PARO

void paro(){
        
        sprintf(missatge,"{M 0}");
        
 }

//FUNCION MARCHA

void marcha(){
            
        llenar_array();
        adquirir_muestra(buffer[7]);        

        sprintf(missatge,"{M 0}");
        
}

//FUNCION PARA ADQUIRIR MUESTRAS EN EL ARRAY

void adquirir_muestra (int N) {
		
    muestra = (float*)malloc(N*sizeof(float*));
    
		if (muestra == NULL) {
			printf("Error array. No se ha podido reservar memoria.\n");
		}
        /*CREA LOS VALORES EN EL ARRAY*/
		else {                
            srand48(time(NULL));
                
            for(i=0; i<N; i++) {
                *(muestra+i)=drand48() * (40.00-10.05) + 10.05;
                cola_circular (muestra,N);
			}
	}
}

//FUNCION DE GUARDAR MUESTRAS EN EL ARRAY
        
void cola_circular (float *muestra, int x) {
    
    float media=0;
        
        n++;
        j=(frente+n)%t_max;              
        media=*(muestra+i)/x;
        datos[j]=media;
}

//FUNCION DE LLENAR EL ARRAY ALEATORIAMIENTE


void llenar_array () {
	
            int N = t_max;
            int j=0;
            
    muestra = (float*)malloc(N*sizeof(float*));
    
		if (muestra == NULL) {
			printf("Error array. No se ha podido reservar memoria.\n");
		}
        /*CREA LOS VALORES EN EL ARRAY*/
		else {                
            
            srand48(time(NULL));
                
            for(i=0; i<N; i++) {
                *(muestra+i)=drand48() * (40.00-10.05) + 10.05;
                j=(frente+n)%t_max;              
                n++;
                datos[j]=*(muestra+i);	
            }
        }
    printf("\nEl array se ha llenado con éxito\n");
}


//FUNCION DE MANIPULACION DE DATOS 

void manipulacion(){
    
            if (strcmp(buffer,"{U}") == 0) {                
                muestra_antigua();
            } 
            else if (strcmp(buffer, "{X}") == 0 ){
                muestra_maxima();
            }
            else if (strcmp(buffer, "{Y}") == 0 ){
                muestra_minima();
            }
            else if (strcmp(buffer, "{R}") == 0 ){
                reset_max_min();
            }
            else if (strcmp(buffer, "{B}") == 0 ){
                numero_muestras_array();
            }
            else if (buffer[1] == 'M') {
                if (buffer[3] == '1'){
                    marcha();
                    }
                
                else {
                paro();
                }
            }
            else{ 
                sprintf(missatge,"{%c 1}",buffer[1]);
            }
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
		
        /*Enviar*/
		strcpy(buffer,missatge); //Copiar missatge a buffer
		result = write(newFd, buffer, strlen(buffer)+1); //+1 per enviar el 0 final de cadena
		printf("Missatge enviat a client(bytes %d): %s\n",	result, missatge);
 
        /*Tancar el socket fill*/
		result = close(newFd);
	}
}




