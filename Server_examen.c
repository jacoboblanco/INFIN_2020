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

//------  nuevas librerias

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>


//------

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
int i;
int j=0;
int unsigned frente=0;
int unsigned n=0;
float *muestra;
float datos[t_max];
float menor;
float mayor;

int	y; //Variable global
        
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
void adquirir_muestra(int N);
void cola_circular(float sumatorio, int x);

//FUNCION MUESTRA ANTIGUA

void muestra_antigua(){

        float ultimo;
        
        if(j < 0){
            sprintf(missatge, "{U2}");
        }
        else{
        ultimo=datos[j];
        sprintf(missatge, "{U0%.2f}",ultimo);
        frente=(frente+1)%t_max;
        n--;
        j--;
        }
   }

//FUNCION RESET MAXIMO Y MINIMO

void reset_max_min(){
        
            mayor=0; 
            menor=0;
            
            sprintf(missatge,"{R0}");
        }

//FUNCION NUMERO MUESTRAS

void numero_muestras_array(){
        
        if(n<t_max){
            sprintf(missatge, "{B0%d}",n);
        }
            
        else{
           sprintf(missatge, "{B0%d}",t_max);
        }
 }

//FUNCION MUESTRA MAXIMA

void muestra_maxima(){

      if(n<0){

            sprintf(missatge, "{X2}");
        }
 
      else{
        
            sprintf(missatge, "{X0%.2f}",mayor);        
        }
}
    
//FUNCION MUESTRA MINIMA

void muestra_minima(){
        
        if(n<0){
            sprintf(missatge, "{Y2}");
            }
       
        else{ 
          
        }
        sprintf(missatge, "{Y0%.2f}",menor);
    }
 
//FUNCION PARO

void paro(){
        
        sprintf(missatge,"{M0}");
 }

//FUNCION MARCHA

void marcha(){
                   
        adquirir_muestra(buffer[4]);        

        sprintf(missatge,"{M0}");
    
    /*LLENAR REGISTRO DE MAYOR */  
        mayor = datos[0];
        
     for (int i=0;i<n;i++){
                if (datos[i]>mayor){
                mayor=datos[i];
                }
            }   
    
    /*LLENAR REGISTRO DE MENOR */
            menor = datos[0];
            
        for (int i=0;i<n;i++){
                if (datos[i]<menor){
                menor=datos[i];
                }
            }     
        }

//FUNCION PARA ADQUIRIR MUESTRAS EN EL ARRAY

void adquirir_muestra (int N) {
		
    float suma=0;
    
    muestra = (float*)malloc(N*sizeof(float*));
    
		if (muestra == NULL) {
			printf("Error array. No se ha podido reservar memoria.\n");
		}
        
        /*CREA LOS VALORES EN EL ARRAY*/
		
        else {                
            srand48(time(NULL));

            for(i=0; i<N; i++) {
                *(muestra+i)=drand48() * (40.00-10.05) + 10.05;
               // printf("%.2f ;",*(muestra+i));
                suma = suma + *(muestra+i);
            }
               // printf("\n%2.f",suma);
                cola_circular (suma,N);
	}
}

//FUNCION DE GUARDAR MUESTRAS EN EL ARRAY
        
void cola_circular (float sumatorio, int x) {
    
    float entrada = 0;    
    
        j=(frente+n)%t_max;              
        entrada = sumatorio/x;
        datos[j]=entrada;
        n++;
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
                if (buffer[2] == '1'){
                    
                    marcha();
                    
                    printf("\nDatos almacenados: ");
                        
                        for(i=0;i<t_max;i++){
                        printf("%.2f ",datos[i]);
                    }
                    
                    printf("\n\n");
                }
                
                else {
                paro();
                }
            }
            else{ 
                sprintf(missatge,"{%c1}",buffer[1]);
            }
   }
            

/************************
*
*
* tcpServidor
*
*
*/
void* codi_fill(void* parametre){ // codi thread fill
   	while(y>0){
  		printf("Thread fill PID(%d) i=%d\n",getpid(), y);
  		y--;
  		sleep(2); // Retard de 2 segons
  	}
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[]) {
    
	pthread_t thread;
	
  y=5;

  printf("Proces pare 1 PID(%d) \n",getpid() );

  pthread_create(&thread, NULL, codi_fill, 0); //Es crea el thread fill

  printf("Proces pare 2 PID(%d) \n",getpid() );// Proces Pare
  	
 	


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
	
	pthread_join(thread, NULL);

 	printf("Proces pare PID(%d)\n",getpid());

  return EXIT_SUCCESS;
}

