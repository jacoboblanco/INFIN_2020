


//cola circular

#include <stdio.h>

int main () {
	// entrada del dato
	int frente=0;
	int tamaño=9;
	int n =0;
	int dades[tamaño]; 	
	printf("introduzca el dato en la cola\n");
	scanf("%i", dato);

	
	if (n==tamaño){
		printf("la cola está llena");
		}
		else {	
			int ubi=(frente+n)%tamaño;
			dades[ubi]=dato;
			n++;
	 }	
	//salida del dato
	int salida= dades[frente];
	printf("la temperatura es: %i",salida);
	(frente=frente+1)%tamaño;
	n--;
	
	

return 0;
}


