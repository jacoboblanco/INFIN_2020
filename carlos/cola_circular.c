







//cola circular

#include <stdio.h>

int main () {
   
   //el problema a solucionar es que cuando no hay valores en la lista 
   //printea las direcciones...
	
	int dades[8];  //vector almacen
    	int ubi=0;     //posicion del vector que se printea
	int s=1;       //STOP
	int S=1;       //STOP
	int d;         //dato que le llega del cliente
	
	int f=0;       //frente
	int t=7;       //tamaño
	int n =0;      //numero de datos
	
	int o=0;       //output
//recepcion del dato


while (s==1){
    printf("introduzaca el valor\n");
    scanf("%i",&d);                       
	printf("\n\n");
			                              
			 ubi=(f+n)%t;   //ubi=posicion del vector rellenamos
			 dades[ubi]=d;  //dades[0,1,2,3,4,5,6]
			 n++;
			//printf("%i\n",ubi);
			
			printf("%d\n",dades[0]);
			printf("%d\n",dades[1]);
			printf("%d\n",dades[2]);
			printf("%d\n",dades[3]);
			printf("%d\n",dades[4]);
			printf("%d\n",dades[5]);
			printf("%d\n",dades[6]);
			
		} 
		
		
		//salida del dato
	
	if(S==1){
			o=dades[f];
			printf("la temperatura es %i",o);
			f=(f+1)%t;
			n--;
		}
			
		
return 0;
}


