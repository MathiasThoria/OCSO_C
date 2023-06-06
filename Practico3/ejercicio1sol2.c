
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
  int mem_id,j,k,m,n,o;     
  mem_id = shmget(clave,sizeof(int),0777|IPC_CREAT);
  
  //int * ;
  //= (int*) shmat(mem_id,NULL,NULL);
  
  sem_t * semaforo;
  semaforo = sem_open ("/misemaforo", O_CREAT, 0644, 1);

  int i = fork();
  
  if (i == -1) exit(0);
  if (i > 0){
    for(o=0;o<100;o++){
      for(j=0;j<100;j++){
        printf("| + "); 
        for(m=0;m<10000;m++) n++;
      }
      printf("<F");
    }
  }else{
    for(o=0;o<100;o++){
      for(k=0;k<100;k++){
        printf("| - ");    
        for(m=0;m<10000;m++) n++;
      }
      printf("<s ");
    }
  } 
}