#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>                                                    
#include <sys/stat.h>                                                     
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
#include <stdio.h>                                                        
#include <strings.h>
#include <unistd.h>
#include <sys/ioctl.h>     
 
#define BAUDRATE B9600                                                
//#define MODEMDEVICE "/dev/ttyS0"        //Conexió IGEP - Arduino
#define MODEMDEVICE "/dev/ttyACM0"         //Conexió directa PC(Linux) - Arduino                                   
#define _POSIX_SOURCE 1 /* POSIX compliant source */                       
    
#define t_max 100                                                           
                                                         
struct termios oldtio,newtio;                                                           
char buf[255];
char missatge[255];                                           
int unsigned frente=0;
int unsigned n=0;
float *muestra;
float datos[t_max];
float menor;
float mayor;
int tiempo;
int nmediana=0;
int j=0;
int fd, i = 0, res;                                                           
float temperatura;		
int w=0;


void guardar();
void cola_circular(float sumatorio, int x);
void adquisicion ();  
void LED ();
void convertidor();
void adquirir_muestra (int N);  
void Enviar(int fd,char *missatge);	//Subrutina per a enviar dades per el port serie
void Rebre(int fd,char *buf);		//Subrutina per a rebre dades per el port serie
int	ConfigurarSerie(void);			//Configuració del port serie obert
void TancarSerie(int fd);			//Tancar comunicació
    
int main(int argc, char **argv)                                                               
{                                                                          

	fd = ConfigurarSerie();                                                                         
                                                                         

	adquisicion();   
	Enviar(fd,missatge);
	sleep(1);
	Rebre(fd,buf);
          
	while(1){ 
      
		LED ();  
		convertidor();
		guardar();
	}   
		                                                      
	TancarSerie(fd);
	
	return 0;
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
	//hay que crear una variable donde esten todo los RES concatenados
	//printf("Rebuts %d bytes: %s\n",res,buf);	//***********************************************************************************************************
}
void LED (){

	if (w==0){
		w=1; 
	}

	else{
		w=0;
	}
		sprintf(missatge,"AS13%dZ",w); 
		Enviar(fd,missatge);
		sleep(1);
		Rebre(fd,buf);
	} 

void adquisicion (){
	
	int v;


	printf("\nSeleccione el modo que desea: 1 (marcha) o 0 (paro): ");
	scanf("%d",&v);		
				
			if(v==1){
				
				printf("\nTiempo de muestreo que desea en segundos: ");
				scanf("%d",&tiempo);
				printf("\nNumero de muestras de las que desea hacer la mediana: ");
				scanf("%d",&nmediana);
				printf("\nSe hara el muestreo de %d segundos y se hara la media con %d muestras\n",tiempo,nmediana);
				sprintf(missatge, "AM%d%.2dZ",v,tiempo);
			}

			else{				
				
				printf("\nHa puesto el modo paro\n");
				sprintf(missatge, "AM000Z");
			}
}		
				
void guardar(){
                   
        adquirir_muestra(nmediana);        

      
    
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
                
        printf("Maximo: %.2f\nMinimo: %.2f\n",mayor,menor);
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
        
            for(i=0; i<N; i++) {
                *(muestra+i)=temperatura;
            	//printf("\nmuestra:%.2f\n",*(muestra+i));
            	suma = suma + *(muestra+i);
            }
                cola_circular (suma,N);
	}
}

//FUNCION DE GUARDAR MUESTRAS EN EL ARRAY
        
void cola_circular (float sumatorio, int x) {
    
    float entrada = 0;    
    
        j=(frente+n)%t_max;              
        entrada = sumatorio/x;
        datos[j]=entrada;
        printf("\nMedia: %.2f\n",entrada);       
        n++;
}
        
void convertidor(){	
 
	char lecturatemp[7];

	float temp;

		sprintf(missatge,"ACZ"); 
		Enviar(fd,missatge);
		sleep(tiempo);    //se queda leiendo el tiempo t previamente introducido 
		Rebre(fd,buf);

		sprintf(lecturatemp,"%c%c%c%c%c",buf[3],buf[4],buf[5],buf[6],buf[7]);

		temp=atof(lecturatemp);  //convierte a float printf("Introduzca un valor entre 0 y 1023\n"); 

		temperatura=temp*5/1023;
		printf("\nMuestra(%d):%.2f\n",n,temperatura);
 }
