#include <stdio.h>
#include <stdlib.h>

#include <semaphore.h>
#include <sys/ipc.h>
#include <fcntl.h>

main(){
  sem_t * semaforo;
  semaforo = sem_open ("/misemaforo", O_CREAT, 0644, 1); /*semafaro nombrado, se puede accede a el mediante el nombre*/
  /*si un semaforo no es nombrado es necesario acceder a el medidante los hilos
  1 significa que es binario. es un tope de acceso.


  */
  int i= fork();
  if (i>0)
    for (int j=0;j<10000;j++){
      //sem_wait(semaforo); 
        printf("Inicio consumo\n");
        printf("Fin consumo\n");
      //sem_post(semaforo);
    }
  else
    for (int j=0;j<10000;j++){
      //sem_wait(semaforo);
        printf("      Inicio productor\n");
        printf("      Fin productor\n");
      //sem_post(semaforo);
    }
  

}