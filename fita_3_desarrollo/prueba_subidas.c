// gcc fita_3_desarrollo.c -o fita3
// ./fita3




/************************************************  FITA 3  **********************************************************/






int  main (){
	

 
	
	
return 0;

}

//librerias para serie(HABRAN ALGUNAS REPETIDAS)***************

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>                                                        
#include <termios.h>       
#include <sys/ioctl.h>    
 
#define BAUDRATE B9600                                                
//#define MODEMDEVICE "/dev/ttyS0"        //Conexió IGEP - Arduino
#define MODEMDEVICE "/dev/ttyACM0"         //Conexió directa PC(Linux) - Arduino                                   
#define _POSIX_SOURCE 1 /* POSIX compliant source */                       
                                                           
struct termios oldtio,newtio;     
//*************************************************************
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

int i;
int j=0;
int unsigned frente=0;
int unsigned n=0;
float *muestra;
float datos[t_max];
float menor;
float mayor;
        
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
void centigrados ();                             //nueva funcion a integrar

//FUNCION MUESTRA ANTIGUA

void muestra_antigua(){    

        float ultimo;
       
        if(j < 0){
         //  sprintf(missatge, "{U2}");
        
        }
        else{
        ultimo=datos[j];
        //sprintf(missatge, "{U0%.2f}",ultimo);
        printf(" el ultimo %f", ultimo);
        frente=(frente+1)%t_max;
        n--;
        j--;
        }
   }

//FUNCION RESET MAXIMO Y MINIMO

void reset_max_min(){
        
            mayor=0; 
            menor=0;
            
           // sprintf(missatge,"{R0}");
           printf(" El mayor %f y el menor %f", mayor,menor);
        }

//FUNCION NUMERO MUESTRAS


void numero_muestras_array(){
        
        if(n<t_max){
          // sprintf(missatge, "{B0%d}",n);
          printf("El maximo es %d",n);
        }
            
        else{
           //sprintf(missatge, "{B0%d}",t_max);
            printf("El minimo es %d",t_max);
        }
 }


//FUNCION MUESTRA MAXIMA

void muestra_maxima(){

      if(n<0){

            //sprintf(missatge, "{X2}");
        
        }
 
      else{
        
            //sprintf(missatge, "{X0%.2f}",mayor);  
             printf("%f",mayor);      
        }
}
    
//FUNCION MUESTRA MINIMA

void muestra_minima(){
        
        if(n<0){
            //sprintf(missatge, "{Y2}");
            }
       
        else{ 
          
        }
        //sprintf(missatge, "{Y0%.2f}",menor);
        printf("%f",menor);
    }
 
//FUNCION PARO

void paro(){
        
        //sprintf(missatge,"{M0}");
 }

//FUNCION MARCHA

void marcha(){
                   
        adquirir_muestra(buffer[4]);        

       // sprintf(missatge,"{M0}");
    
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

void adquirir_muestra (int N) {     //AQUI VA LA COMUNICACIÓN SERIE CON EL CONTROL SENSOR
/*		
    float suma=0;
    
    muestra = (float*)malloc(N*sizeof(float*));
    
		if (muestra == NULL) {
			printf("Error array. No se ha podido reservar memoria.\n");
		}
        
        //CREA LOS VALORES EN EL ARRAY
		
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
*/
}

//FUNCION DE GUARDAR MUESTRAS EN EL ARRAY
        
void cola_circular (float sumatorio, int x) {
    
    float entrada = 0;    
    
        j=(frente+n)%t_max;              
        entrada = sumatorio/x;
        datos[j]=entrada;
        n++;
}


void centigrados (){   //SE TIENEN QUE CAMBIAR LAS VARIABLES PARA LA LECTURA
	
	float lectura;
	float voltios;

	printf("Introduzca un valor entre 0 y 1023\n");
	scanf("%f",&lectura);
	voltios=lectura*5/1023;
	printf("%f", voltios);
		
}
int	ConfigurarSerie(void)
{
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

void TancarSerie(int fd)
{
	tcsetattr(fd,TCSANOW,&oldtio);
	close(fd);
}

void Enviar(int fd,char *missatge)
{
	int res=0;
	
	res = write(fd,missatge,strlen(missatge));
	
	if (res <0) {tcsetattr(fd,TCSANOW,&oldtio); perror(MODEMDEVICE); exit(-1); }
	
	//printf("Enviats %d bytes: %s\n",res,missatge);	//***********************************************************************************************************
}
void Rebre(int fd,char *buf)
{
	int k = 0;
	int res = 0;
	int bytes = 0;
	
	ioctl(fd, FIONREAD, &bytes);

	do
	{
		res = res + read(fd,buf+k,1);
		k++;
	}
	while (buf[k-1] != 'Z');	//PARA CUANDO LEA EL FINAL DEL PROTOCOLO
	//printf("Rebuts %d bytes: %s\n",res,buf);	//***********************************************************************************************************
}
