

//Funcion array de muestras

#include <stdio.h>


void lectura (int a[]);
void retorno (int a[]);

int main() {
 
	int dades[10];
	
	lectura(dades);
	retorno(dades);
	
	
return 0;	
}

void lectura (int a[]){      			//pide la introducción de unos valores

    int i;	
	for(i=0;i<10;i++){
		printf("introduce el valor");
		scanf("%i", &a[i]);
		}
}

void retorno (int a[]){   				//printea unos valores
	int i;
	for(i=0;i<10;i++){
		printf("%i", a[i]);
		}
	
}
