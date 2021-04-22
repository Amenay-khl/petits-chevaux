#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

typedef struct Tabpipe Tabpipe;

struct Tabpipe{
  int pipe_write;
  int pipe_read;
  int pipe_pf;
  Tabpipe *suiv;
};

typedef struct Listep Listep;

struct Listep{
  Tabpipe *tete;
};

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


void pos(int x,Listep *tp){
  int i=1;
  while(i<x){
    tp->tete=tp->tete->suiv;
    i++;
  }
}

void closeall(){
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
  if (pipe(j23)==-1){
    perror ("pipe");
    exit(-1);
  }
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
  //pipe pour désigner le joueur qui joue (entre père et joueur)
  if (pipe(jo1)==-1){
    perror ("pipe");
    exit(-1);
  }
  if (pipe(jo2)==-1){
    perror ("pipe");
    exit(-1);
  }
  if (pipe(jo3)==-1){
    perror ("pipe");
    exit(-1);
  }
  if (pipe(jo4)==-1){
    perror ("pipe");
    exit(-1);
  }
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

void process_fils2(int x,Listep *tp){
  switch(fork()){
    case -1: //gestion cas erreur
    fprintf(stderr,"erreur de fork\n");
    exit(-1);
    case 0: //comportement du fils
    printf("%d\n",i);
    pos(i,tp);
    char tmp [4] ;
    sprintf (tmp, "%d", i) ;
    printf("tmp %s\n",tmp);
    //lecture du joueur qui joue
    read(tp->tete->pipe_read,val,4);
    printf("init reçu 2 %s\n",val);
    write(tp->tete->pipe_write,val,4);
    printf("init envoie 2 %s\n",val);
    while(strcmp(val,"5")!=0){
      read(tp->tete->pipe_pf,data,4);
      printf("data 2 %s\n",data);
      //Si joueur 2
      if (strcmp(data,"1")==0){
        //A adapter selon les cas
        if (tab[i-1]==1 && tab[i-2]==1){
          //envoie signal fin de partie au père
          printf("fini 2\n");
          write(tp->tete->pipe_write,"5",4);
          read(tp->tete->pipe_read,val,4);
        }
        else{
          write(final[1],"0",4);
          //envoie de l'info que joueur 2 a joué à joueur 3
          write(tp->tete->pipe_write,tmp,4);
          printf("envoyé %d %s\n",i,tmp);
          //Lecture du retour de l'info par le joueur 1
          read(tp->tete->pipe_read,val,4);
          printf("reçu %d: %s\n",i,val);
          //envoie signal fin de tour au père
          write(tour[1],"1",4);
          //ajout d'un tour joué par le joueur
          tab[i-1]+=1;
          printf("tab %d %d\n",i,tab[i-1]);
        }
        }
        else{
          //transmission de l'info venant du joueur 1 vers le joueur 3
          read(tp->tete->pipe_read,val,4);
          printf("transmission %d: %s\n",i,val);
          tab[atoi(val)-1]+=1;
          write(tp->tete->pipe_write,val,4);
        }
      }
      write(final[1],"1",4);
      write(tour[1],"2",4);
      printf("close2\n");
      closeall();
      printf("exit2\n");
      exit(0);
      default:  //comportement du père
      if (x>1){
        i++;
        process_fils2(x-1,tp);}
    }
  }

void process_fils1(int x,Listep *tp){
    switch(fork()){
      case -1: //gestion cas erreur
      fprintf(stderr,"erreur de fork\n");
      exit(-1);
      case 0: //comportement du fils
      write(final[1],"0",4);
      //lecture du joueur qui joue
      write(tp->tete->pipe_write,"0",4);
      printf("init envoie\n");
      read(tp->tete->pipe_read,val,4);
      printf("init reçu %s\n",val);
      while(strcmp(val,"5")!=0){
        printf("val 1 %s\n",val);
        read(tp->tete->pipe_pf,data,4);
        printf("boucle\n");
        //Si joueur 1
        printf("data 1 %s\n",data);
        if (strcmp(data,"1")==0){
          printf("jeu 1\n");
          //A adapter selon les cas
          printf("x : %d\n",x);
          printf("tab %d\n",tab[x-1]);
          if (tab[0]==1 && tab[x-1]==1){
            //envoie signal fin de partie au père
            printf("fini 1\n");
            write(tp->tete->pipe_write,"5",4);
            read(tp->tete->pipe_read,val,4);
          }
          else{
            write(final[1],"0",4);
            //envoie de l'info que joueur 1 a joué à joueur 2
            write(tp->tete->pipe_write,"1",4);
            printf("envoyé 1\n");
            //Lecture du retour de l'info par le joueur 4
            read(tp->tete->pipe_read,val,4);
            printf("reçu 1: %s\n",val);
            //envoie signal fin de tour au père
            write(tour[1],"1",4);
            //ajout d'un tour joué par le joueur
            tab[0]+=1;
          }
          }
          else{
            printf("jeu 2\n");
            //transmission de l'info venant du joueur 4 vers le joueur 2
            read(tp->tete->pipe_read,val,4);
            printf("transmission 1: %d\n",atoi(val));
            tab[atoi(val)-1]+=1;
            write(tp->tete->pipe_write,val,4);
          }
        }
        write(final[1],"1",4);
        write(tour[1],"2",4);
        printf("close1\n");
        closeall();
        printf("exit1\n");
        exit(0);
        default:  //comportement du père
        process_fils2(x-1,tp);
      }
  }

  int main (int argc, char *argv[]){
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
    printf("Combien de joueur voulez vous ? (entre 2 et 4) : ");
    scanf("%d",&x0);
    printf("\n");
    while (x0<2 || x0>4){
      printf("erreur du nombre de joueur\n");
      printf("Combien de joueur voulez vous ? (entre 2 et 4) : ");
      scanf("%d",&x0);
      printf("\n");
    }
    //création des pipes
    initall(x0,tp,tp1,tp2,tp3,tp4);
    //création de joueur
    process_fils1(x0,tp);
    switch(jp){
      case 1:
      printf("cas 1\n");
      write(jo1[1],"1",4);
      write(jo2[1],"0",4);
      write(jo3[1],"0",4);
      write(jo4[1],"0",4);
      break;
      case 2:
      printf("cas 2\n");
      write(jo2[1],"1",4);
      write(jo1[1],"0",4);
      write(jo3[1],"0",4);
      write(jo4[1],"0",4);
      break;
      case 3:
      printf("cas 3\n");
      write(jo3[1],"1",4);
      write(jo1[1],"0",4);
      write(jo2[1],"0",4);
      write(jo4[1],"0",4);
      break;
      case 4:
      printf("cas 4\n");
      write(jo4[1],"1",4);
      write(jo1[1],"0",4);
      write(jo3[1],"0",4);
      write(jo2[1],"0",4);
      break;
    }
    read(final[0],f,4);
    while (strcmp(f,"1")!=0){
      read(tour[0],tf,4);
      //changement de joueur
      if (strcmp(tf,"1")==0){
        jp+=1;
        if (jp==x0+1){
          jp-=x0;
        }

      switch(jp){
        case 1: write(jo1[1],"1",4);
        write(jo2[1],"0",4);
        write(jo3[1],"0",4);
        write(jo4[1],"0",4);
        break;
        case 2: write(jo2[1],"1",4);
        write(jo1[1],"0",4);
        write(jo3[1],"0",4);
        write(jo4[1],"0",4);
        break;
        case 3: write(jo3[1],"1",4);
        write(jo1[1],"0",4);
        write(jo2[1],"0",4);
        write(jo4[1],"0",4);
        break;
        case 4: write(jo4[1],"1",4);
        write(jo1[1],"0",4);
        write(jo3[1],"0",4);
        write(jo2[1],"0",4);
        break;
      }}
      read(final[0],f,4);
      printf("final %s\n",f);
    }
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
    closeall();
    return 0;
  }
