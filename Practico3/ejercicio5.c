#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
// constantes de secuencias de escape ANSI para formatear salida
#define ROJO "\x1b[31m"
#define VERDE "\x1b[32m"
#define NORMAL "\x1b[0m"
#define TOPE_COORD 50

// Construya una solución en la cual no es posible generar nuevas coordenadas muestras se están generando otras. 
//  Asuma que se la restricción anterior se elimina, ¿cambia la solución?, justifique.

// como se pueden generar nuevas coordenadas si es 1 sistema el que las genera?
// significa que tengo que hacer que otro proceso tambien genere coordenadas?




int testeoQueConsumeTiempo(){
  int i,num;
  for (i = 0; i <= 20000000; i++)
    num++;
  return 1;
}

int main(){
  key_t clave;
  int mem_id, resultado, j, k;

  int *x;
  clave = ftok("/home/maths/REPOS/Repos_INET/Practico3/ejercicio5", 25);
  // printf("key.%d.",clave);
  mem_id = shmget(clave, sizeof(int), 0777 | IPC_CREAT);
  x = (int *)shmat(mem_id, NULL, NULL); // memoria compartida
  *x = 0;

  int *y;
  clave = ftok("/home/maths/REPOS/Repos_INET/Practico3/ejercicio5", 26);
  // printf("key.%d.",clave);
  mem_id = shmget(clave, sizeof(int), 0777 | IPC_CREAT);
  y = (int *)shmat(mem_id, NULL, NULL); // memoria compartida
  *y = 0;

  // variable para parar while's
  int *stopy;
  clave = ftok("/home/maths/REPOS/Repos_INET/Practico3/ejercicio5", 27);
  // printf("key.%d.",clave);
  mem_id = shmget(clave, sizeof(int), 0777 | IPC_CREAT);
  stopy = (int *)shmat(mem_id, NULL, NULL); // memoria compartida
  *stopy = 0;

  sem_t *semaforo;
  semaforo = sem_open("/misemaforo1", O_CREAT, 0644, 1);

  // PARA SABER VALOR DE SEMAFORO
  // si un semaforo quedo "colgado" por kill o Ctr C
  int *getval;
  getval = (int *)malloc(sizeof(int));
  *getval = -1;
  sem_getvalue(semaforo, getval);
  printf("||%d||", *getval);
  if (*getval == 0){
    sem_post(semaforo);
    sem_getvalue(semaforo, getval);
    printf("onerror||%d||", *getval);
  }
  ////////

  int i = fork();

  if (i > 0){ // proceso toma y guarda coordenadas
    
    for (j = 0; j < TOPE_COORD/2; j++){
      sleep(1);
      sem_wait(semaforo);
      *x = rand() % 80; // numero entre 0 y 10
      *y = rand() % 80;
      printf(ROJO"\nGENERADO -> %d, %d"NORMAL, *x, *y);
      fflush(stdout);
      sem_post(semaforo);
    }
  }else{

    int f = fork();
    if (f > 0){ // proceso muestra coordenadas
      
      for (k = 0; k < 10; k++) {
        sleep(3);
        sem_wait(semaforo);
        if ((*x != 0) || (*y = 0)){
          printf("\n%d %d", *x, *y);
          fflush(stdout);
        }
        else
          printf("\nCoordenada invalida");

        sem_post(semaforo);
      }
    }else{ 
      for (j = 0; j < TOPE_COORD/2; j++){
        sleep(3);
        sem_wait(semaforo);
          *x = rand() % 80; // numero entre 0 y 10
          *y = rand() % 80;
          printf(VERDE"\nGENERADO -> %d, %d"NORMAL, *x, *y);
          fflush(stdout);
        sem_post(semaforo);
      }
    }
  }

}