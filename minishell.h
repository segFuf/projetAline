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
};

enum {
	in = 0,
	out = 1,
	err = 2
};

typedef struct ENVIRONEMENT {
	int fd[3];
	char *dirs[MaxDirs];
	int in[2];
	int out[2];
} env_t;



void execCommande(char *mot[], int t, env_t *env);
int execChaine(char *commandes, env_t *env);
int execution(char *commandes, env_t *env, int multi);
int execPipe(char *commandes, int multi, env_t *env);
int execBasic(char *commandes, env_t *env);

int decouper(char * ligne, char * separ, char * mot[], int maxmot); /* decouper  --  decouper une chaine en mots */
int findChaine(char *str); // trouve si il y a && dans la commande, et donc plusieur commande en faite.
int fd_Commande(char *str); // cherche si c'est un commande que j'ai codé moi méme
int my_cd(int , char *[]); // le cd de la prof
int my_exit(int , char *[]); // la fonction exit qui quitte le shell
int command_empty(int a, char *path[]);
int (**get_tab_commande(void))(int , char *[]); // renvoie le tableau de pointeur sur les fonctions des commandes que j'ai codé moi méme/ (cd,exit pour l'instant)
int set_redirections(char *mot[], int nb_arg, env_t *env);
int set_std_red(char *mot[], int nb_arg, env_t *env);
int findPipe(char *str);
void clear_pipe(env_t *env);
#endif