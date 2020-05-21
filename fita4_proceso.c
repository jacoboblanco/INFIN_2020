/***************************************************************************
                          main.c  -  threads
                             -------------------
    begin                : ene 30 19:49:08 CET 2002
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

#include <stdlib.h>
#include <sys/types.h>                                                    
#include <sys/stat.h>                                                     
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>                                            
#include <fcntl.h>                                                        
#include <termios.h>                                                      
#include <stdio.h>                                                        
#include <strings.h>
#include <sys/ioctl.h>     
#include <time.h>

#define SERVER_PORT_NUM		5001
#define SERVER_MAX_CONNECTIONS	4

#define REQUEST_MSG_SIZE	1024
 
#define BAUDRATE B9600                                                
//#define MODEMDEVICE "/dev/ttyS0"        //Conexió IGEP - Arduino
#define MODEMDEVICE "/dev/ttyACM0"         //Conexió directa PC(Linux) - Arduino                                   
#define _POSIX_SOURCE 1 /* POSIX compliant source */                       
    
#define t_max 100                 //Tamaño máximo del array de muestras      

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
int h=0;
struct termios oldtio,newtio;                                                           
char buffer_arduino[255];         //Espacio de memoria para guardar lo que recibe del arduino
char missatge_arduino[255];       //Protocolo que se envia al arduino                            
int unsigned frente=0;            //Posición 0 de la cola circular
int unsigned n=0;                 //Número de muestra
float *muestra;                   //Vector dinámico que se utiliza en la cola circular
float datos[t_max];               //Vector donde se guardan los datos en la cola circular
float menor;                      //Variable para guardar la muestra mínima
float mayor;                      //Variable para guardar la muestra máxima
int nmediana=0;                   //Número de muestras con el que se quiere hacer la muestra
int j=0;                          //Contador que marca la posición a la que apunta la cola circular
int fd, i = 0, res;               //Variables que se utilizan para comunicación serial                                             
int w=0;                          //Variable que se utiliza para encender y apagar el LED

//FUNCIONES UTILIZADAS

void Guardar(float temperatura);                      //Función que realiza la lecutra, guarda la muestra, llena los registros de máx mín, y calcula la media
void Cola_circular(float sumatorio, int x);           //Gurada el valor de la media en el vector circular.
void Adquisicion();                                   //Función que lee los valores que se introducen por la terminal (tiempo, media, marcha y paro)
void LED();                                           //Función para pagar y encender el LED
float Convertidor();                                  //Convierte la muestra de digital a grados centígrados
void Adquirir_muestra (int N,float temperatura);      //Recoge los datos que le llegan del arduino
void Enviar(int fd,char *missatge_arduino);           //Función que se uttiliza para enviar datos por el puerto serie
void Rebre(int fd,char *buffer_arduino);		      //Función que se uttiliza para recibir datos por el puerto serie
int	ConfigurarSerie(void);			                  //Configuración del puerto serie
void TancarSerie(int fd);			                  //Función que se utiliza para cerrar la comunicación con el puerto serie
float Media(int numero);                              //Función que realiza la media de las muestras especificadas en la terminal
void seleccion();
void muestra_antigua();
void muestra_maxima();
void muestra_minima();
void reset_max_min();
void numero_muestras_array();
void paro();
void marcha();

void* codi_fill(void* parametre){ // codi thread fill
   
    float temperatura=0;
    float mediana=0;
    int h_anterior=h;
	fd = ConfigurarSerie();                                                                                                                                            
    
    /*Se utiliza un while(1) para crear un bucle itinerante*/      
	while(1){ 
        
        if (h==1 && h_anterior==0){
            Enviar(fd,missatge_arduino);
	        Rebre(fd,buffer_arduino);
        }
        
		LED ();  
		temperatura=Convertidor();
		Guardar(temperatura);
		mediana=Media(nmediana);
		//printf("\nMedia: %.2f\n",mediana); 
	}   
		                                                      
	TancarSerie(fd);
    pthread_exit(NULL);

    return NULL;
}   


int main(int argc, char *argv[])
{
	pthread_t thread;
	

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
		
        seleccion();
		
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
/***************************** FUNCIONES FITA 3 DESARROLLADAS ***********************/

int	ConfigurarSerie(void){
	int fd;                                                           


	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );                             
	if (fd <0) {perror(MODEMDEVICE); exit(-1); }                            

	tcgetattr(fd,&oldtio); /* save current port settings */                 

	bzero(&newtio, sizeof(newtio));                                         
	//newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;             
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;             
	newtio.c_iflag = IGNPAR;                                                
	newtio.c_oflag = 0;                                                     

	/* set input mode (non-canonical, no echo,...) */                       
	newtio.c_lflag = 0;                                                     

	newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */         
	newtio.c_cc[VMIN]     = 1;   /* blocking read until 1 chars received */ 

	tcflush(fd, TCIFLUSH);                                                  
	tcsetattr(fd,TCSANOW,&newtio);
	
		
 	sleep(2); //Per donar temps a que l'Arduino es recuperi del RESET
		
	return fd;
}               

void TancarSerie(int fd) {
	tcsetattr(fd,TCSANOW,&oldtio);
	close(fd);
}  
              
void Enviar(int fd,char *missatge_arduino) {
	int res=0;
	
	res = write(fd,missatge_arduino,strlen(missatge_arduino));
	
	if (res <0) {tcsetattr(fd,TCSANOW,&oldtio); perror(MODEMDEVICE); exit(-1); }
	
	//printf("Enviats %d bytes: %s\n",res,missatge_arduino);
}
              
void Rebre(int fd,char *buffer_arduino) {
	int k = 0;
	int res = 0;
	int bytes = 0;
	
	ioctl(fd, FIONREAD, &bytes);

	do
	{
		res = res + read(fd,buffer_arduino+k,1);
		k++;
	}
	while (buffer_arduino[k-1] != 'Z');	
	
	//printf("Rebuts %d bytes: %s\n",res,buffer_arduino);
}

void LED() {

	if (w==0){
		w=1; 
	}

	else{
		w=0;
	}
		sprintf(missatge_arduino,"AS13%dZ",w); 
		Enviar(fd,missatge_arduino);
		sleep(1);
		Rebre(fd,buffer_arduino);
	} 
				
void Guardar(float temperatura) {
                   
     Adquirir_muestra(nmediana, temperatura);        
    
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
                
        //printf("Maximo: %.2f\nMinimo: %.2f\n",mayor,menor);
    }

float Media(int numero) {

	float suma=0;
	float m=0;
	int x=0;
	float valor=0;
        
		for(i=0; i<numero; i++) {
			x=n-i;
			valor=datos[x];
			suma = suma + valor; 
		}
     
	m = suma/numero;
	
	return m;

}

void Adquirir_muestra(int N, float temperatura) {
		
    float suma=0;

    muestra = (float*)malloc(N*sizeof(float*));

		if (muestra == NULL) {
			printf("Error array. No se ha podido reservar memoria.\n");
		}
		
        else {                
        
            for(i=0; i<N; i++) {
                *(muestra+i)=temperatura;
            	suma = suma + *(muestra+i);
            }
                Cola_circular (suma,N);
	}
}
        
void Cola_circular(float sumatorio, int x) {
    
    float entrada = 0;    
    
        j=(frente+n)%t_max;              
        entrada = sumatorio/x;
        datos[j]=entrada;
       
        n++;
}
        
float Convertidor() {	
	
	float temperatura;      //Variable que se utiliza para pasar el valor de la temperatura a grados centígrados
	char lecturatemp[7];

	float temp;

		sprintf(missatge_arduino,"ACZ"); 
		Enviar(fd,missatge_arduino);
		sleep(buffer[3]);    
		Rebre(fd,buffer_arduino);

		sprintf(lecturatemp,"%c%c%c%c%c",buffer_arduino[3],buffer_arduino[4],buffer_arduino[5],buffer_arduino[6],buffer_arduino[7]);

		temp=atof(lecturatemp);  //convierte a float

		temperatura=temp*5/1023;
		//printf("\nMuestra(%d):%.2f\n",n,temperatura);
		
	return temperatura; 
 }

/***************FUNCIONES DE LA FITA 1 DESARROLLADAS*************************************************************/

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

void reset_max_min(){
        
            mayor=0; 
            menor=0;
            
            sprintf(missatge,"{R0}");
        }

void numero_muestras_array(){
        
        if(n<t_max){
            sprintf(missatge, "{B0%d}",n);
        }
            
        else{
           sprintf(missatge, "{B0%d}",t_max);
        }
 }

void muestra_maxima(){

      if(n<0){

            sprintf(missatge, "{X2}");
        }
 
      else{
        
            sprintf(missatge, "{X0%.2f}",mayor);        
        }
}
    
void muestra_minima(){
        
        if(n<0){
        
            sprintf(missatge, "{Y2}");
        }
            
        else{ 
          
            sprintf(missatge, "{Y0%.2f}",menor);
        }
    }
 

void paro(){
        
        printf("\nHa puesto el modo paro\n");
		
        sprintf(missatge_arduino, "AM000Z");
        sprintf(missatge,"{M0}");
 }

void marcha(){
    
    int tiempo_entrado_usuario=((buffer[3])*10+(buffer[4]));
                   //{M1023}
        sprintf(missatge_arduino, "AM1%dZ",tiempo_entrado_usuario);
        sprintf(missatge,"{M0}");
}

void seleccion(){
    
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
                    h=1;

                    printf("\nDatos almacenados: ");
                        
                        for(i=0;i<t_max;i++){
                        printf("%.2f ",datos[i]);
                    }
                    
                    printf("\n\n");
                }
                
                else {
                paro();
                h=0;
                }
            }
            else{ 
                sprintf(missatge,"{%c1}",buffer[1]);
            }
   }
