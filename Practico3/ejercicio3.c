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


//NOTAS:
//no tengo claro que valores reinicio al finalizar el ciclo de productos o llegar a 1000 de masa
//solucion 1 semaforos por fuera del while/dowhile (sin prodMasa  e iterador compartidos)

//interpete que seria mas "rapido" el "trabajo" se realizaba entre
//dos "personas". Para ello era necesario que ambos vean peso total y 
//cuantos productos llevaban "procesando"
//eso es sincronia?

//BUGS:
//sucede que se me "escapa" alguna iteracion al final. no entiendo porque, el if esta dentro de
//semaforo

//ojo mem_id -> si reservo memoria con el mismo memid se generan 2 variables con
// con la misma direccion

/*sem_wait()  decrements  (locks) the semaphore pointed to by sem.  If the semaphore's value
       is greater than zero, then the decrement proceeds, and the function returns,  immediately.
       If  the  semaphore  currently  has  the  value  zero, then the call blocks until either it
       becomes possible to perform the decrement (i.e., the semaphore value rises above zero), or
       a signal handler interrupts the call.*/



#define MAX_BALANZA 1000
#define CONJUNTOS 3 //para repetir varias veces el conjunto de 20. ojo que comparador k
                    // no compartido (repite en ambos procesos)
#define MASA_BASE 80

int testeoQueConsumeTiempo(int num){
  int i;
  for (i=0;i<=20000000;i++) num++;
  return 1;
} 


int main () {

  key_t clave;
  clave = ftok("/home/maths/REPOS/Repos_INET/Practico3/ejercicio3",25);
  printf("key.%d.",clave);
  int mem_id;
  int todoOk,espacioArr;     
 
  
  espacioArr = sizeof(int)*20;
  printf("espacio:%d",espacioArr);
  mem_id = shmget(clave,espacioArr,0777|IPC_CREAT);
  
  /*perror("Error en shm");
  printf(" memid:%d ",mem_id);
  if(mem_id < 0){
    printf("Error en shmget\n"); 
    return -1;
  }*/

  
  int* prodMasa;
  prodMasa = (int*) shmat(mem_id,NULL,0); //memoria compartida
  int j;
  for (j=0;j<=20;j++){ 
    prodMasa[j]=MASA_BASE;
  }

  //llego a necesitar la clave o la mem_id y march{e}
  //contador compartido de cuantos de los 20 articulos van
  int* iterador;
  clave = ftok("/home/maths/REPOS/Repos_INET/Practico3/ejercicio3",26);
  mem_id = shmget(clave,sizeof(int),0777|IPC_CREAT);
  iterador = (int*) shmat(mem_id,NULL,NULL); //memoria compartida
  *iterador = 0;

  //contador de masa compartido
  int* masaTotal;
  clave = ftok("/home/maths/REPOS/Repos_INET/Practico3/ejercicio3",27);
  mem_id = shmget(clave,sizeof(int),0777|IPC_CREAT);
  masaTotal = (int*) shmat(mem_id,NULL,NULL); //memoria compartida
  *masaTotal = 0;

  int* k;
  clave = ftok("/home/maths/REPOS/Repos_INET/Practico3/ejercicio3",28);
  mem_id = shmget(clave,sizeof(int),0777|IPC_CREAT);
  k = (int*) shmat(mem_id,NULL,NULL); //memoria compartida
  *k = 0;
  //
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
  //-------------------------------------------------

  int i = fork();

  while (*k <= CONJUNTOS){ 
      
    printf("\n INICIO en {%d}- Masa total anterior:%d",i,*masaTotal);
    printf("\n Conjunto n°:%d",*k);    
    

    //ACA LOOPEO SEGUN REALIDAD
    if ((*masaTotal+prodMasa[*iterador])>MAX_BALANZA){
     *masaTotal=0;
    }

    
    if (*iterador>19){
      *iterador=0;       
      *k=*k+1; 
    }           
  
  
    if (i > 0){ //porque tengo que distinguir los procesos si hacen casi lo mismo?        

      do{                 
      //problema-> si uso while ((*masaTotal<=MAX_BALANZA) && (*iterador<=20))      
      // repite y compara en la misma sentencia. necesito que compare dentro del sem 
      // y repita afuera para que permita intervenir al otro proceso
   
        sem_wait(semaforo); //cuando espera guarda las variables de ese momento?
          printf("\n\nDentro de semaforo: -%d-",*masaTotal);
          fflush(stdout);  
          if (((*masaTotal+prodMasa[*iterador])<MAX_BALANZA) && (*iterador<=19)){
            printf("\n{%d}",i);      
            todoOk = testeoQueConsumeTiempo(prodMasa[*iterador]);             // interpreto que testea de uno iterando
            if (todoOk) printf(ROJO" Producto %d testeado."NORMAL,*iterador);
                
            *masaTotal += prodMasa[*iterador]; 
            printf("Masa total: %d",*masaTotal);        
            *iterador=*iterador+1;  
            fflush(stdout);
              
          }
        sem_post(semaforo);
        
      }while (((*masaTotal+prodMasa[*iterador])<MAX_BALANZA)&&(*iterador<=19)) ;
      
      wait();
      sem_destroy(semaforo);
    
    }else{

      do{                            
        sem_wait(semaforo);      
          printf("\n\nDentro de semaforo: -%d-",*masaTotal);
          if (((*masaTotal+prodMasa[*iterador])<MAX_BALANZA) && (*iterador<=19)){
            printf("\n    {%d}",i);        
            todoOk = testeoQueConsumeTiempo(prodMasa[*iterador]);             // interpreto que testea de uno iterando
            if (todoOk) printf(VERDE" Producto %d testeado."NORMAL,*iterador);
              
            *masaTotal += prodMasa[*iterador];
            printf("Masa total: %d",*masaTotal);        
            *iterador=*iterador+1;
            fflush(stdout);             
          }
        sem_post(semaforo);        
      }while (((*masaTotal+prodMasa[*iterador])<MAX_BALANZA) && (*iterador<=19));      
    }
  }
  
  printf("\n");
  return 1;
}