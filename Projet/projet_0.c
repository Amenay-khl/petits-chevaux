#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void process_fils(int x){
  if (x>0){
    switch(fork()){
      case -1: //gestion cas erreur
      fprintf(stderr,"erreur de fork\n");
      exit(-1);
      case 0: //comportement du fils
      printf("fils %d: %d\n",x,getpid());
      exit(0);

      default:  //comportement du père
      process_fils(x-1);
    }
  }
}

int main (int argc, char *argv[]){
  int x;
  printf("Combien de joueur voulez vous ? : ");
  scanf("%d",&x);
  printf("\n");
  printf("père : %d\n",getppid());
  process_fils(x);
  return 0;

}
