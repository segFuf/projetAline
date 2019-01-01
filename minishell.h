#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum {
  MaxLigne = 1024,              // longueur max d'une ligne de commandes
  MaxMot = MaxLigne / 2,        // nbre max de mot dans la ligne
  MaxDirs = 100,		// nbre max de repertoire dans PATH
  MaxPathLength = 512,		// longueur max d'un nom de fichier
  MaxCommandes = MaxMot / 2,
  MaxHist = 40,
};

enum {
	in = 0,
	out = 1,
	err = 2
};

typedef struct ENVIRONEMENT {
	int processus;		// variable de retour du fork()
	int fd[3];		// stdout, stdin, stderr
	char *dirs[MaxDirs];	// tout les PATH
	int in[2];		// pipe in
	int out[2];		// pipe out
	int (**commandes)(int , char *[], struct ENVIRONEMENT *env);	// un tableau de pointeur qui contient cd et exit
} env_t;



void execCommande(char *mot[], int t, env_t *env); 	// execute une commande
int execChaine(char *commandes, env_t *env);		// decoupe une chaine de &&
int execution(char *commandes, env_t *env, int multi);	// set up avant d'appeler execCommand
int execPipe(char *commandes, int multi, env_t *env);	// decoupe et set up pour une chaine de pipe
int execBasic(char *commandes, env_t *env);		// une ou plusieur commande entre ;

int decouper(char * ligne, char * separ, char * mot[], int maxmot); /* decouper  --  decouper une chaine en mots */
int findChaine(char *str); // trouve si il y a && dans la commande, et donc plusieur commande en faite.
int fd_Commande(char *str); // cherche si c'est un commande que j'ai codé moi méme
int my_cd(int , char *[], env_t *env); // le cd de la prof
int my_exit(int , char *[], env_t *env); // la fonction exit qui quitte le shell
int command_empty(int a, char *path[], env_t *env);
int (**get_tab_commande(void))(int , char *[], env_t *env); // renvoie le tableau de pointeur sur les fonctions des commandes que j'ai codé moi méme/ (cd,exit pour l'instant)
int set_redirections(char *mot[], int nb_arg, env_t *env); // set up une redirection >> 2> &>
int set_std_red(char *mot[], int nb_arg, env_t *env); // set up une redirection 1>&2 2>&1
int findPipe(char *str); // cherche si le caractere | est dans la str
void clear_pipe(env_t *env); // reset les pipes de env
#endif