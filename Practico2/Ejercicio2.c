#include <stdio.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>
// ojaldre que la letra dice iterar 100 veces
main () {
  key_t clave;
  clave = ftok("/home/maths/INETrepos/shm3procesos.tmp",25);
  int mem_id;  
  mem_id = shmget(clave,1024,0777|IPC_CREAT);
  int *a;
  a = (int*) shmat(mem_id,NULL,NULL);

  int i = fork();
  *a = 0;  
  if (i == -1) exit(0);
  if (i > 0){
    while (1){
      for (int j=0; j<100; j++);   
      *a = *a * 0;
      printf("%i-",*a);       
    }
  }else{
    int i = fork();
    if (i == -1) exit(0);
    if (i > 0){
      while (1){
      for (int j=0; j<100; j++);   
      *a = *a + 1;
      printf("%i-",*a);       
      }
    }else{
      while (1){
        for (int j=0; j<100; j++);   
        *a = *a / 2;
        printf("%i-",*a);       
      }
    }
  }    
}