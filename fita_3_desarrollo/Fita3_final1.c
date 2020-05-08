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
    
#define t_max 100                 //Tamaño máximo del array de muestras      
                                    
//VARIABLES GLOBALES                                                         

struct termios oldtio,newtio;                                                           
char buf[255];                    //Espacio de memoria para guardar lo que recibe del arduino
char missatge[255];               //Protocolo que se envia al arduino                            
int unsigned frente=0;            //Posición 0 de la cola circular
int unsigned n=0;                 //Número de muestra
float *muestra;                   //Vector dinámico que se utiliza en la cola circular
float  datos[t_max];               //Vector donde se guardan los datos en la cola circular
float menor;                      //Variable para guardar la muestra mínima
float mayor;                      //Variable para guardar la muestra máxima
int tiempo;                       //Tiempo en el que se hacen las muestras
int nmediana=0;                   //Número de muestras con el que se quiere hacer la muestra
int j=0;                          //Contador que marca la posición a la que apunta la cola circular
int fd, i = 0, res;               //Variables que se utilizan para comunicación serial                                             
int w=0;                          //Variable que se utiliza para encender y apagar el LED

//FUNCIONES UTILIZADAS

void guardar(float temperatura);                                 //Función que realiza la lecutra, guarda la muestra, llena los registros de máx mín, y calcula la media
void cola_circular(float sumatorio, int x);     //Gurada el valor de la media en el vector circular.
void adquisicion ();                            //Función que lee los valores que se introducen por la terminal (tiempo, media, marcha y paro)
void LED ();                                    //Función para pagar y encender el LED
float convertidor();                             //Convierte la muestra de digital a grados centígrados
void adquirir_muestra (int N,float temperatura);                  //Recoge los datos que le llegan del arduino
void Enviar(int fd,char *missatge);         	//Función que se uttiliza para enviar datos por el puerto serie
void Rebre(int fd,char *buf);		            //Función que se uttiliza para recibir datos por el puerto serie
int	ConfigurarSerie(void);			            //Configuración del puerto serie
void TancarSerie(int fd);			            //Función que se utiliza para cerrar la comunicación con el puerto serie
float Media(int numero);


 int main(int argc, char **argv) {                                                                          
 
 float temperatura=0;
 float mediana=0;
 
	fd = ConfigurarSerie();                                                                                                                                            

	adquisicion();   
	Enviar(fd,missatge);
	Rebre(fd,buf);
    
    /*Se utiliza un while(1) para crear un bucle itinerante*/
         
	while(1){ 
      
		LED ();  
		temperatura=convertidor();
		guardar(temperatura);
		mediana=Media(nmediana);
		printf("\nMedia: %.2f\n",mediana); 
	}   
		                                                      
	TancarSerie(fd);
	
	return 0;
}           		

int	ConfigurarSerie(void) {
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
              
void Enviar(int fd,char *missatge) {
	int res=0;
	
	res = write(fd,missatge,strlen(missatge));
	
	if (res <0) {tcsetattr(fd,TCSANOW,&oldtio); perror(MODEMDEVICE); exit(-1); }
	
	//printf("Enviats %d bytes: %s\n",res,missatge);
}
              
void Rebre(int fd,char *buf) {
	int k = 0;
	int res = 0;
	int bytes = 0;
	
	ioctl(fd, FIONREAD, &bytes);

	do
	{
		res = res + read(fd,buf+k,1);
		k++;
	}
	while (buf[k-1] != 'Z');	
	
	//printf("Rebuts %d bytes: %s\n",res,buf);
}

void LED() {

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

void adquisicion() {
	
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
				
void guardar(float temperatura) {
                   
     adquirir_muestra(nmediana, temperatura);        
    
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
void adquirir_muestra(int N, float temperatura) {
		
    float suma=0;

    muestra = (float*)malloc(N*sizeof(float*));

		if (muestra == NULL) {
			printf("Error array. No se ha podido reservar memoria.\n");
		}
		
        else {                
        
            for(i=0; i<N; i++) {
                *(muestra+i)=temperatura;
            	//printf("\nmuestra:%.2f\n",*(muestra+i));
            	suma = suma + *(muestra+i);
            }
                cola_circular (suma,N);
	}
}
        
void cola_circular(float sumatorio, int x) {
    
    float entrada = 0;    
    
        j=(frente+n)%t_max;              
        entrada = sumatorio/x;
        datos[j]=entrada;
       
        n++;
}
        
float convertidor() {	
	
	float temperatura;      //Variable que se utiliza para pasar el valor de la temperatura a grados centígrados
	char lecturatemp[7];

	float temp;

		sprintf(missatge,"ACZ"); 
		Enviar(fd,missatge);
		sleep(tiempo);    
		Rebre(fd,buf);

		sprintf(lecturatemp,"%c%c%c%c%c",buf[3],buf[4],buf[5],buf[6],buf[7]);

		temp=atof(lecturatemp);  //convierte a float

		temperatura=temp*5/1023;
		printf("\nMuestra(%d):%.2f\n",n,temperatura);
		
	return temperatura; 
 }
