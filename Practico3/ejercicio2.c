
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
//constantes de secuencias de escape ANSI para formatear salida
#define ROJO "\x1b[31m"
#define VERDE "\x1b[32m"
#define NORMAL "\x1b[0m"


void main () {

  key_t clave;
  clave = ftok("/home/maths/INETrepos/Practico3/ejercicio2.tmp",25);
  int mem_id,j,k,m;     
  mem_id = shmget(clave,sizeof(int),0777|IPC_CREAT);
  
  int * variablecompartida;
  variablecompartida = (int*) shmat(mem_id,NULL,NULL);
  *variablecompartida = 0;
  
  sem_t * semaforo;
  semaforo = sem_open ("/misemaforo1", O_CREAT, 0644, 1);
    
  //PARA SABER VALOR DE SEMAFORO
  //int * getval;
  //getval = (int*)malloc(sizeof(int));
  //*getval=0;
  //sem_getvalue(semaforo,getval);
  //printf("||%d||",*getval);
  //if (getval==0) sem_post(semaforo);  

  int i = fork();
  if (i > 0){
    for(j=0; j<1000;j++){       // PRODUCTOR
      //for(k=0; k<10000000;k++) m++;  
      sem_wait(semaforo);        
        *variablecompartida += 1;        
        printf(ROJO"((%d))"NORMAL,*variablecompartida);
        fflush(stdout);        
      sem_post(semaforo);  
    }
    //wait();
    sem_destroy(semaforo);
  }else{
    for(j=0;j<1000;j++){
      //for(k=0;k<100000000;k++) m++;
      sem_wait(semaforo); // CONSUMIDOR        
        if ((*variablecompartida%2)==0){
          printf(VERDE"-%d-"NORMAL,*variablecompartida);
          fflush(stdout);
        }                  
      sem_post(semaforo);      

    }
  }
}