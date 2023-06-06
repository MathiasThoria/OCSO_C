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

int testeoQueConsumeTiempo(int num){
  int i;
  for (i=0;i<=20000000;i++) num++;
  return 1;
} 


int main () {
  key_t clave;
  int mem_id,resultado;

  int* num1;  
  clave = ftok("/home/maths/REPOS/Repos_INET/Practico3/ejercicio4",25);
  //printf("key.%d.",clave);  
  mem_id = shmget(clave,sizeof(int),0777|IPC_CREAT);  
  num1 = (int*) shmat(mem_id,NULL,NULL); //memoria compartida
  *num1 = 0;

  int* num2;  
  clave = ftok("/home/maths/REPOS/Repos_INET/Practico3/ejercicio4",26);
  //printf("key.%d.",clave);  
  mem_id = shmget(clave,sizeof(int),0777|IPC_CREAT);  
  num2 = (int*) shmat(mem_id,NULL,NULL); //memoria compartida
  *num2 = 0;

  //variable para parar while's
  int* stopy;  
  clave = ftok("/home/maths/REPOS/Repos_INET/Practico3/ejercicio4",27);
  //printf("key.%d.",clave);  
  mem_id = shmget(clave,sizeof(int),0777|IPC_CREAT);  
  stopy = (int*) shmat(mem_id,NULL,NULL); //memoria compartida
  *stopy = 0;

  sem_t * semaforo;
  semaforo = sem_open ("/misemaforo1", O_CREAT, 0644, 1);

  //PARA SABER VALOR DE SEMAFORO
  //si un semaforo quedo "colgado" por kill o Ctr C
  int * getval;
  getval = (int*)malloc(sizeof(int));
  *getval=-1;
  sem_getvalue(semaforo,getval);
  printf("||%d||",*getval);
  if (*getval==0){
    sem_post(semaforo);  
    sem_getvalue(semaforo,getval);
    printf("onerror||%d||",*getval);
  }
  ////////

  int i = fork();
  
  if (i > 0){                 //proceso azar 
    
    while((*stopy) == 0){
      //sleep(1);
      sem_wait(semaforo);
        *num1 = rand() % 10; //numero entre 0 y 100
        *num2 = rand() % 10;
        printf(NORMAL"\nGENERADO -> %d, %d",*num1,*num2);
        fflush(stdout);
      sem_post(semaforo);
    }   
  }else{    

    int f = fork(); 
    
    if (f > 0){     
      int g = fork(); 
      
      if (g > 0){             //proceso muestra suma        
        while((*stopy) == 0){
          //sleep(1);
          sem_wait(semaforo);
            resultado=*num1+*num2;
            printf(VERDE"\n%d+%d=%d"NORMAL,*num1,*num2,resultado);
            fflush(stdout);
          sem_post(semaforo);
        }              
      
      }else{                  //proceso muestra resta
        while((*stopy) == 0){
          //sleep(1);
          sem_wait(semaforo);
            resultado=*num1-*num2;
            printf(ROJO"\n%d-%d=%d"NORMAL,*num1,*num2,resultado);
            fflush(stdout);
          sem_post(semaforo);
        }  
      
      }     
    }else{                    //proceso stop
      getchar();
      printf("||Stoped.||");
      *stopy = 1;
    }
  }
}