//fork realiza una copia del codigo

#include <stdio.h>

main() {
  int i=fork();
  if (i==-1) exit(0);                              // Controla la ocurrencia de un error
  if (i>0){                                                  // Este es el proceso padre
    for (int j=0; j<1000000000; j++){
        printf("+");
        /*wait(i);*/
    }
  }
  else                                                       // Este es el proceso hijo
      for (int s=0; s<1000000000; s++)
              printf("-");
}

/*
#Include <stdio.h>

main() {
  int i=fork();
  int a=0;
  If (i==-1)  exit(0);               
  if (i>0) {                      
      for (int j=0; j<100; j++)
              printf(“%i”,a);
      a=1; }
  else                           
      for (int s=0; s<100; s++)
              printf(“%i”,a);
}

*/