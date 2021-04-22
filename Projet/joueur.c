#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "petit_chevaux.h"

extern int j12[2];
extern int j23[2];
extern int j34[2];
extern int j41[2];
extern int jo1[2];
extern int jo2[2];
extern int jo3[2];
extern int jo4[2];
extern int tour[2];
extern int final[2];
extern char f[4];
extern int jp;
extern char val[4];
extern char tf[4];
extern char data[4];
extern int tab[4];
extern int x0;
extern int i;

//définie la tete de la liste chainée selon le joueur
void pos(int x,Listep *tp){
  int i=1;
  while(i<x){
    tp->tete=tp->tete->suiv;
    i++;
  }
}

//Création des joueurs 2 à 3
void process_fils2(int x,Listep *tp){
  switch(fork()){
    case -1: //gestion cas erreur
    fprintf(stderr,"erreur de fork\n");
    exit(-1);
    case 0: //comportement du fils
    srand( time( NULL )+getpid()  );
    int de;
    printf("%d\n",i);
    //positionnement de la tete de la liste chainée
    pos(i,tp);
    //transformation de i en tampon
    char tmp [4] ;
    char tmp2 [4] ;
    sprintf (tmp, "%d", i) ;
    printf("tmp %s\n",tmp);
    //Lancée de dé jusqu'à qu'un joueur fasse 6
    read(tp->tete->pipe_pf,data,4);
    while(strcmp(data,"5")!=0){
      if (strcmp(data,tmp)==0){
        de=(rand() % 7);
        printf("dé %s %d\n",tmp,de);
        sprintf (tmp2, "%d", de) ;
        write(final[1],tmp2,4);
      }
      read(tp->tete->pipe_pf,data,4);
    }
    //Vérification de la communication entre joueurs et initialisation de val
    read(tp->tete->pipe_read,val,4);
    printf("init reçu 2 %s\n",val);
    write(tp->tete->pipe_write,val,4);
    printf("init envoie 2 %s\n",val);
    while(strcmp(val,"5")!=0){
      //attente de la désignation du joueur qui joue
      read(tp->tete->pipe_pf,data,4);
      printf("data 2 %s\n",data);
      //Si joueur actuel
      if (strcmp(data,tmp)==0){
        //fin de partie
        if (tab[i-1]==1 && tab[i-2]==1){
          //envoie signal fin de partie aux joueurs
          printf("fini 2\n");
          write(tp->tete->pipe_write,"5",4);
          read(tp->tete->pipe_read,val,4);
        }
        else{
          //partie non finie
          write(final[1],"0",4);
          //envoie de l'info que joueur actuel a joué au joueur suivant
          write(tp->tete->pipe_write,tmp,4);
          printf("envoyé %d %s\n",i,tmp);
          //Lecture du retour de l'info par le joueur actuel
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
          //transmission de l'info venant du joueur précédent vers le joueur suivant
          read(tp->tete->pipe_read,val,4);
          printf("transmission %d: %s\n",i,val);
          //mise à jour du tableau de tour
          tab[atoi(val)-1]+=1;
          write(tp->tete->pipe_write,val,4);
        }
      }
      //envoie signal fin de partie au père
      write(final[1],"1",4);
      write(tour[1],"2",4);
      printf("close2\n");
      //Fermeture des pipes
      closeall();
      printf("exit2\n");
      //Suppresion joueur
      exit(0);
      default:  //comportement du père
      //Création d'autres joueurs si besoin
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
      srand( time( NULL )+getpid()  );
      int de;
      char tmp [4] ;
      //initialisation boucle père
      write(final[1],"0",4);
      //Lancée de dé jusqu'à qu'un joueur fasse 6
      read(tp->tete->pipe_pf,data,4);
      while(strcmp(data,"5")!=0){
        if (strcmp(data,"1")==0){
          de=(rand() % 7);
          printf("dé 1 %d\n",de);
          sprintf (tmp, "%d", de) ;
          write(final[1],tmp,4);
        }
        read(tp->tete->pipe_pf,data,4);
      }
      //initialisation et vérification des communication entre joueurs
      write(tp->tete->pipe_write,"0",4);
      printf("init envoie\n");
      read(tp->tete->pipe_read,val,4);
      printf("init reçu %s\n",val);
      while(strcmp(val,"5")!=0){
        printf("val 1 %s\n",val);
        //attente de la désignation du joueur qui joue
        read(tp->tete->pipe_pf,data,4);
        printf("boucle\n");
        //Si joueur 1
        printf("data 1 %s\n",data);
        if (strcmp(data,"1")==0){
          printf("jeu 1\n");
          printf("x : %d\n",x);
          printf("tab %d\n",tab[x-1]);
          //fin de partie
          if (tab[0]==1 && tab[x-1]==1){
            //envoie signal fin de partie aux joueurs
            printf("fini 1\n");
            write(tp->tete->pipe_write,"5",4);
            read(tp->tete->pipe_read,val,4);
          }
          else{
            //envoie de non fin de partie au père
            write(final[1],"0",4);
            //envoie de l'info que joueur 1 a joué à joueur 2
            write(tp->tete->pipe_write,"1",4);
            printf("envoyé 1\n");
            //Lecture du retour de l'info par le joueur précédent
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
            //transmission de l'info venant du joueur précédent vers le joueur 2
            read(tp->tete->pipe_read,val,4);
            printf("transmission 1: %d\n",atoi(val));
            //maj du tableau de tour
            tab[atoi(val)-1]+=1;
            write(tp->tete->pipe_write,val,4);
          }
        }
        //envoie signal fin de partie au père
        write(final[1],"1",4);
        write(tour[1],"2",4);
        printf("close1\n");
        //Fermeture des pipes
        closeall();
        printf("exit1\n");
        //Suppression joueur
        exit(0);
        default:  //comportement du père
        //Création de joueur 2
        process_fils2(x-1,tp);
      }
  }
