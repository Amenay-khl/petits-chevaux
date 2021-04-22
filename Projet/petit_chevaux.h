#ifndef _PETIT_CHEVAUX
#define _PETIT_CHEVAUX

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

void closeall();

void initall(int x,Listep *tp,Tabpipe *tp1,Tabpipe *tp2,Tabpipe *tp3,Tabpipe *tp4);


#endif
