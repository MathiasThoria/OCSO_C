/*Sea un sistema de control de operaciones bancarias, el cual permite hasta 100 operaciones netas. Cada vez que
 se realiza una operación de retiro el número de operaciones netas se incrementa en 1, si se realiza un depósito 
 el número de operaciones netas se decrementa en 1. Los retiros son 5 veces más frecuentes que los depósitos. 
 Cuando las operaciones netas llegan a 100 todo el sistema se detiene. Cree un programa que modele la realidad anterior.

En ocasiones, y con las herramientas trabajadas hasta ahora, este sistema puede no funcionar correctamente, ¿por qué?

En todos los casos recuerde incluir operaciones de espera.*/


#include <stdio.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>

//constantes de secuencias de escape ANSI para formatear salida
#define ROJO "\x1b[31m"
#define VERDE "\x1b[32m"
#define NORMAL "\x1b[0m"



int randomMovement(){
  // Convertir el número aleatorio en 1 o -1
  // -1 para deposito, 1 para retiro
  // Frecuencia controlada por numeroAleatorio >= 0,2

  int numeroAleatorio;
  int randy;
  randy = rand() % 10;
  //printf("(%d)",randy);
  if (randy <= 2)
    numeroAleatorio = -1; //deposito    
  else
    numeroAleatorio = 1; //retiro    
  
  return numeroAleatorio;
}


main () {

  key_t clave;
  clave = ftok("/home/maths/INETrepos/shm3procesos.tmp",25);
  int mem_id, tmpMov;   
  
  mem_id = shmget(clave,sizeof(int),0777|IPC_CREAT);
  
  int * movimientosDelDia;
  movimientosDelDia = (int*) shmat(mem_id,NULL,NULL);
  *movimientosDelDia = 0;  

  int i = fork();
  
  if (i == -1) exit(0);
  if (i > 0){
    while (*movimientosDelDia < 100) {      
      tmpMov = randomMovement(); //me devuelve +/- 1 (retiro/deposito) con un frecuencia acorde a la letra.
      *movimientosDelDia += tmpMov; //modifico memoria compartida
      if (tmpMov > 0) printf("+"); //para acomodar salida
      printf(ROJO "%d | %d |" NORMAL,tmpMov,*movimientosDelDia);    //colores para identificar proceso
      printf("PADRE \n");
    }
  }else{
    while (*movimientosDelDia < 100) {
      tmpMov = randomMovement();
      *movimientosDelDia += tmpMov;
      if (tmpMov > 0) printf("+"); 
      printf(VERDE "%d | %d |" NORMAL,tmpMov,*movimientosDelDia);    
      printf("HIJO \n");
    }
  } 
}
  