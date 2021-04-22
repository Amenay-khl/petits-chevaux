#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "petit_chevaux.h"
#include "joueur.h"

int j12[2];
int j23[2];
int j34[2];
int j41[2];
int jo1[2];
int jo2[2];
int jo3[2];
int jo4[2];
int tour[2];
int final[2];
char f[4];
int jp=1;
char val[4];
char tf[4];
char data[4];
int tab[4]={0,0,0,0};
int x0;
int i=2;


void closeall(){
  //fermeture des pipes
  printf("fctclose\n");
  close(j12[0]);
  close(j12[1]);
  close(j23[0]);
  close(j23[1]);
  close(j34[0]);
  close(j34[1]);
  close(j41[0]);
  close(j41[1]);
  close(tour[0]);
  close(tour[1]);
  close(final[0]);
  close(final[1]);
  close(jo1[0]);
  close(jo1[1]);
  close(jo2[0]);
  close(jo2[1]);
  close(jo3[0]);
  close(jo3[1]);
  close(jo4[0]);
  close(jo4[1]);
}

void initall(int x,Listep *tp,Tabpipe *tp1,Tabpipe *tp2,Tabpipe *tp3,Tabpipe *tp4){
  //pipe pour la communication entre joueur 1 et 2
  if (pipe(j12)==-1){
    perror ("pipe");
    exit(-1);
  }
  //pipe pour la communication entre joueur 2 et 3
  if (pipe(j23)==-1){
    perror ("pipe");
    exit(-1);
  }
  //pipe pour la communication entre joueur 3 et 4
  if (pipe(j34)==-1){
    perror ("pipe");
    exit(-1);
  }
  //pipe pour la communication entre joueur 4 et 1
  if (pipe(j41)==-1){
    perror ("pipe");
    exit(-1);
  }
  //pipe pour dire la fin d'un tour (entre joueur et père)
  if (pipe(tour)==-1){
    perror ("pipe");
    exit(-1);
  }
  //pipe pour dire la fin d'une partie (entre joueur et père)
  if (pipe(final)==-1){
    perror ("pipe");
    exit(-1);
  }
  //pipe pour désigner le joueur 1
  if (pipe(jo1)==-1){
    perror ("pipe");
    exit(-1);
  }
  //pipe pour désigner le joueur 2
  if (pipe(jo2)==-1){
    perror ("pipe");
    exit(-1);
  }
  //pipe pour désigner le joueur 3
  if (pipe(jo3)==-1){
    perror ("pipe");
    exit(-1);
  }
  //pipe pour désigner le joueur 4
  if (pipe(jo4)==-1){
    perror ("pipe");
    exit(-1);
  }
  //Mise en place d'une liste chainée avec des pipes en élément
  //La taille dépend du nombre de joueurs
  switch (x){
    case 2:
    tp2->pipe_write=j41[1];
    tp2->pipe_read=j12[0];
    tp2->pipe_pf=jo2[0];
    tp2->suiv=NULL;
    tp1->pipe_write=j12[1];
    tp1->pipe_read=j41[0];
    tp1->pipe_pf=jo1[0];
    tp1->suiv=tp2;
    tp->tete=tp1;
    break;
    case 3:
    tp3->pipe_write=j41[1];
    tp3->pipe_read=j23[0];
    tp3->pipe_pf=jo3[0];
    tp3->suiv=NULL;
    tp2->pipe_write=j23[1];
    tp2->pipe_read=j12[0];
    tp2->pipe_pf=jo2[0];
    tp2->suiv=tp3;
    tp1->pipe_write=j12[1];
    tp1->pipe_read=j41[0];
    tp1->pipe_pf=jo1[0];
    tp1->suiv=tp2;
    tp->tete=tp1;
    break;
    case 4:
    tp4->pipe_write=j41[1];
    tp4->pipe_read=j34[0];
    tp4->pipe_pf=jo4[0];
    tp4->suiv=NULL;
    tp3->pipe_write=j34[1];
    tp3->pipe_read=j23[0];
    tp3->pipe_pf=jo3[0];
    tp3->suiv=tp4;
    tp2->pipe_write=j23[1];
    tp2->pipe_read=j12[0];
    tp2->pipe_pf=jo2[0];
    tp2->suiv=tp3;
    tp1->pipe_write=j12[1];
    tp1->pipe_read=j41[0];
    tp1->pipe_pf=jo1[0];
    tp1->suiv=tp2;
    tp->tete=tp1;
  }
}

int main (){
  Tabpipe *tp1;
  tp1=(Tabpipe*)malloc(sizeof(Tabpipe));
  Tabpipe *tp2;
  tp2=(Tabpipe*)malloc(sizeof(Tabpipe));
  Tabpipe *tp3;
  tp3=(Tabpipe*)malloc(sizeof(Tabpipe));
  Tabpipe *tp4;
  tp4=(Tabpipe*)malloc(sizeof(Tabpipe));
  Listep *tp;
  tp=(Listep*)malloc(sizeof(Listep));
  //demande du nombre de joueur
  printf("Combien de joueur voulez vous ? (entre 2 et 4) : ");
  scanf("%d",&x0);
  printf("\n");
  while (x0<2 || x0>4){
    printf("erreur du nombre de joueur\n");
    printf("Combien de joueur voulez vous ? (entre 2 et 4) : ");
    scanf("%d",&x0);
    printf("\n");
  }
  //création des pipes et de la liste chainée de pipes
  initall(x0,tp,tp1,tp2,tp3,tp4);
  //création des joueurs
  process_fils1(x0,tp);
  //tranformation de jp en tampon
  char tmp1 [4] ;
  sprintf (tmp1, "%d", jp) ;
  //initialisation de f et vérification communication
  read(final[0],f,4);
  //Lancée de dé jusqu'à qu'un joueur fasse 6
  while(strcmp(f,"6")!=0){
    write(jo1[1],tmp1,4);
    write(jo2[1],tmp1,4);
    write(jo3[1],tmp1,4);
    write(jo4[1],tmp1,4);
    read(final[0],f,4);
    if(strcmp(f,"6")!=0){
    jp+=1;
    if (jp==x0+1){
      jp-=x0;
    }
    sprintf (tmp1, "%d", jp) ;}
    else{
      write(jo1[1],"5",4);
      write(jo2[1],"5",4);
      write(jo3[1],"5",4);
      write(jo4[1],"5",4);
    }
  }
  sprintf (tmp1, "%d", jp) ;
  //désigne joueur qui commence
  write(jo1[1],tmp1,4);
  write(jo2[1],tmp1,4);
  write(jo3[1],tmp1,4);
  write(jo4[1],tmp1,4);
  while (strcmp(f,"1")!=0){
    //attend fin de tour
    read(tour[0],tf,4);
    //changement de joueur
    if (strcmp(tf,"1")==0){
      jp+=1;
      if (jp==x0+1){
        jp-=x0;
      }
      sprintf (tmp1, "%d", jp) ;
      //désigne joueur qui joue après
      write(jo1[1],tmp1,4);
      write(jo2[1],tmp1,4);
      write(jo3[1],tmp1,4);
      write(jo4[1],tmp1,4);
    }
    //vérifie si partie finie
    read(final[0],f,4);
    printf("final %s\n",f);
  }
  //attente de la fin des fils
  switch (x0) {
    case 2:
    wait(NULL);
    wait(NULL);
    break;
    case 3:
    wait(NULL);
    wait(NULL);
    wait(NULL);
    break;
    case 4:
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    break;
  }
  printf("closef\n");
  //Fermeture des pipes
  closeall();
  return 0;
}
