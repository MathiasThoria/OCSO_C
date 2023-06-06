#include <stdio.h>

main() {
  int i=fork();
  if (i==-1) exit(0);                             
  if (i>0){ 
    for (int j=1; j<=10; j++){
      printf("\x1b[31m %d ",j);     
      wait(2);   
    }
  }
  else      
    for (int s=20; s<=30; s++){
      printf("\x1b[36m %d ",s);
    }
}