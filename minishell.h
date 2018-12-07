#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>

int decouper(char * ligne, char * separ, char * mot[], int maxmot); /* decouper  --  decouper une chaine en mots */
int findChaine(char *str); // trouve si il y a && dans la commande, et donc plusieur commande en faite.
int fd_Commande(char *str); // cherche si c'est un commande que j'ai codé moi méme
int my_cd(int , char *[]); // le cd de la prof
int my_exit(int , char *[]); // la fonction exit qui quitte le shell
int command_empty(int a, char *path[]);
int (**get_tab_commande(void))(int , char *[]); // renvoie le tableau de pointeur sur les fonctions des commandes que j'ai codé moi méme/ (cd,exit pour l'instant)
#endif