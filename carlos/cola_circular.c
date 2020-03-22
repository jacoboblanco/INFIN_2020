//cola circular

#include <stdio.h>
void recepcion ();
void envia ();
void max ();
void min ();

	int dades[8];  //vector almacen
    int ubi=0;     //posicion del vector que se printea
	int s=11;       //STOP
	int S=1;       //STOP
	int d;         //dato que le llega del cliente
	int f=0;       //frente
	int t=7;       //tamaño
	int n =0;      //numero de datos
	int o=0;       //output
    int v;
    int mayor=0;
	int menor=100000000;

int main () {
    recepcion();
	envia();
	max();
    min();
return 0;
}

void recepcion (){
	printf ("pulse 69 para activar la adquisicion");
	scanf("%i",&s);
	while (s==69){
		printf("introduzaca el valor\n");
		scanf("%i",&d);                       
		printf("\n\n");
											  
				 ubi=(f+n)%t;   //ubi=posicion del vector rellenamos
				 dades[ubi]=d;  //dades[0,1,2,3,4,5,6]
				 n++;
				int i;
				for (i=0;i<=ubi;i++){
					printf("%d\n",dades[i]);
					}
				
			} 
			
}

void envia (){	
	printf("Introduzca el numero de datos a sacar:");
    scanf("%i",&v);
	int i;
	for (i=0; i<v; i++){
		
	  if(S==1){
				o=dades[f];
				printf("la temperatura es %d\n",o);
				f=(f+1)%t;
				n--;
		}
	}
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
	printf("El valor mayor es: %i\n",menor);
}









