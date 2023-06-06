
#include <stdio.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
//constantes de secuencias de escape ANSI para formatear salida
#define ROJO "\x1b[31m"
#define VERDE "\x1b[32m"
#define NORMAL "\x1b[0m"


main () {

  key_t clave;
  clave = ftok("/home/maths/INETrepos/Practico3/ejercicio1.tmp",25);
  int mem_id,j,k;     
  mem_id = shmget(clave,sizeof(int),0777|IPC_CREAT);
  
  //int * ;
  //= (int*) shmat(mem_id,NULL,NULL);
  
  sem_t * semaforo;
  semaforo = sem_open ("/misemaforo", O_CREAT, 0644, 1);

  int i = fork();
  if (i > 0){
    while (1){
    sem_wait(semaforo); 
      for(j=0;j<100;j++){
          printf("| + ");       
      }
    }
    sem_post(semaforo);
    printf("<F");
  }else{
    sem_wait(semaforo);
    while (1){  
      for(k=0;k<100;k++){      
        printf("| - ");      
      }
    }
    sem_post(semaforo);
    printf("<s ");
  } 
}
