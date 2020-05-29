/***************************************************************************
                          main.c  -  process
                             -------------------
    begin                : ene 30 19:04:25 CET 2002
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int	i; //Variable global que se ve afectada en el proceso de padre y de hijo independientemente de como varie en el proceso en cuestion


int main(int argc, char *argv[]){
  pid_t pid;
	int retorn = 0 ;
	
  i=10; //inicializa contador

  printf("Proces pare PID(%d) \n",getpid() );

  pid = fork(); // A partid d'aqui hi ha dos processos

  printf("Proces ? PID(%d) \n",getpid() );

  if(pid==0){ // Proces fill que resta un valor cada vez que se ejecuta al contador

  	while(i>0){
  		printf("Proces fill PID(%d) i=%d\n",getpid(), i);
  		i--;
  		sleep(1); // Retard de 1 segons
  	}
    exit(1); //Retornem un 1 ???? Compte!

  }else{ // Proces Pare incrementa el contador cada vez que se ejecuta
  	
  	printf("Proces pare PID(%d) i=%d\n",getpid(), i);

  	while(i<20){  
  		printf("Proces pare PID(%d) i=%d\n",getpid(), i);
  		i++;
  		sleep(1); // Retard de 1 segons
  	}

    wait(&retorn);
 		printf("Proces pare PID(%d) retorn=%d\n",getpid(), retorn);
  }

  return EXIT_SUCCESS;
}
