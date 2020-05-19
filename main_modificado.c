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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int	i; //Variable global

void* codi_fill1(void* parametre){ // codi thread fill
   	while(i>0){
  		printf("Thread fill 1 PID(%d) i=%d\n",getpid(), i);
  		i--;
  		sleep(2); // Retard de 2 segons
  	}
    pthread_exit(NULL);
    return NULL;
}

void* codi_fill2(void* parametre){ // codi thread fill
   	while(i>3){
  		printf("Thread fill 2 PID(%d) i=%d-----------------------",getpid(), i);
  		printf("hola soy el nuevo trhead xd\n");
  		sleep(2); // Retard de 2 segons
  	}
    pthread_exit(NULL);
    return NULL;
}


int main(int argc, char *argv[])
{
	pthread_t thread;
	
  i=5;

  printf("Proces pare 1 PID(%d) \n",getpid() );

  pthread_create(&thread, NULL, codi_fill1, 0); //Es crea el thread fill 1
  pthread_create(&thread, NULL, codi_fill2, 0); //Es crea el thread fill 2

  printf("Proces pare 2 PID(%d) \n",getpid() );// Proces Pare
  	
 	while(i<10){
  		printf("Proces pare PID(%d) i=%d\n",getpid(), i);
  		i++;
  		sleep(1); // Retard de 1 segons
  	}

  pthread_join(thread, NULL);

 	printf("Proces pare PID(%d)\n",getpid());

  return EXIT_SUCCESS;
}
