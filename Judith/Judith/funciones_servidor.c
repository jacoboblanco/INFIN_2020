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

void modo ();
void adquirir_muestra (); //Simula los datos obtenidos por el sensor de una manera aleatoria
void siguiente_muestra (); //Funcion que permite introducir una nueva muestra
void cola_circular (int *muestra, int ubi);
void max ();
void min ();

	int s; //Variable para activar modo MARCHA/PARO
	int mayor=0;
	int menor=100000000;
	
 //Para recrear los datos obtenidos del sensor se utilizará un vector/array dinámico circular
 //Primero crearemos el vector dinámico con las siguientes variables:
	int i; //Contador
	int N=7;  //Tamaño de la muestra (modificable)
	int NN=3; //Tamaño segunda muestra
	int *muestra; //Datos que le llega del "sensor" (muestra) -------> vector dinámico
	
//A continuacion crearemos el vector circular: 
	int dades[7];  //vector almacen/mostres adquirides
   	int ubi=0;     //posicion del vector que se printea
	int t=7;       //tamaño
	int f=0;       //frente
	int n =0;      //numero de datos
	
	
int main () {
    modo();
    adquirir_muestra();
    siguiente_muestra ();
    max();
    min();
	
return 0;
}

void modo () {
	do {
	printf ("Para activar modo MARCHA pulse 1. Para activar modo PARO pulse 0. ");
	scanf("%i",&s);
	} while (s!=1 && s!=0);
	
}

void adquirir_muestra () {
	 
	if (s==1){
		printf ("\nHa activado el modo MARCHA. \n");
	
		printf("El tamaño de la PRIMERA MUESTRA es de 7 lecturas. ");

		muestra = (int*)malloc(N*sizeof(int*));
		
		if (muestra == NULL) {
			printf("\nNo se ha podido reservar memoria.\n");
		}
		else {
			srand(time(NULL));
			for(i=0; i<N; i++) {
				*(muestra+i)=rand()%100;
			}
			printf("\n\nValores MUESTRA 1: \n");
			
			for(i=0; i<N; i++) {
				printf("%i ", *(muestra+i));
			}
			printf("\n");
			
			for(i=0; i<N; i++) {
				printf("\nMuestras que se han guardado en el array: \n");	
			
				cola_circular (muestra, ubi);
			}
			printf("\n\n");
		}					
	}
	
	else {
		printf ("\nHa activado el modo PARO, fin de la ejecución del programa. \n\n");
			
	}
}

void siguiente_muestra () {
	
	printf("El tamaño de la SEGUNDA MUESTRA es de 3 lecturas. \nEstas se sobreescribiran en el array. ");

		muestra = (int*)malloc(NN*sizeof(int*));
		
		if (muestra == NULL) {
			printf("\nNo se ha podido reservar memoria.\n");
		}
		else {
			srand(time(NULL));
			for(i=0; i<NN; i++) {
				*(muestra+i)=rand()%90;
			}
			printf("\n\nValores MUESTRA 2: \n");
			
			for(i=0; i<NN; i++) {
				printf("%i ", *(muestra+i));
			}
			printf("\n");
			
			for(i=0; i<NN; i++) {
				printf("\nMuestras que se han guardado en el array, DESPUES de la MUESTRA 2: \n");	
			
				cola_circular (muestra, ubi);
			}
			printf("\n\n");
		}		
}

void cola_circular (int *muestra, int ubi) {
	
	ubi=(f+n)%t;              //ubi=posicion del vector rellenamos
	n++;
	dades[ubi]=*(muestra+i);  //dades[0,1,2,3,4,5,6]
	
	printf("%d, ",dades[0]);
	printf("%d, ",dades[1]);
	printf("%d, ",dades[2]);
	printf("%d, ",dades[3]);
	printf("%d, ",dades[4]);
	printf("%d, ",dades[5]);
	printf("%d, ",dades[6]);
	
}

void max (){

    for (int i=0;i<t;i++){
		if (dades[i]>mayor){
			mayor=dades[i];
		}
	}
	printf("El valor mayor es: %i\n",mayor);
}

void min (){

    for (int i=0;i<t;i++){
		if (dades[i]<menor){
			menor=dades[i];
		}
	}
	printf("El valor menor es: %i\n",menor);
}

