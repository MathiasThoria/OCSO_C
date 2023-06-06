#include <stdio.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>

main () {
  key_t clave;
  clave = ftok("/home/maths/INETrepos/shm3procesos.tmp",25);
  int mem_id;  
  mem_id = shmget(clave,1024,0777|IPC_CREAT);
  int *banca;
  banca = (int*) shmat(mem_id,NULL,NULL);

  int i = fork();
  *banca = 0;  
  if (i == -1) exit(0);
  if (i > 0){
    while (*banca <= 1000){
      for (int j=0; j<100000; j++);   
      *banca = *banca + 10;
      printf(" A |");
    }
  }else{
    int i = fork();
    if (i == -1) exit(0);
    if (i > 0){
      while (*banca <= 1000){
        for (int j=0; j<100000; j++);
        *banca = *banca + 10;
        printf(" B |");
      }
    }else{
      while (*banca <= 1000){
        for (int j=0; j<1000; j++);
        *banca = *banca + 10;
        printf(" C |");
      }    
    }
  }    
}