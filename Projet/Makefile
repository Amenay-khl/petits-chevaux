
petit_chevaux: petit_chevaux.o joueur.o
	gcc -g -o petit_chevaux petit_chevaux.o joueur.o

petit_chevaux.o: petit_chevaux.c petit_chevaux.h joueur.h
	gcc -g -o petit_chevaux.o -c petit_chevaux.c -W -Wall

joueur.o: joueur.c joueur.h
	gcc -g -o joueur.o -c joueur.c -W -Wall

clean:
	rm -rf *.o
