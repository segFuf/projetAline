#include "minishell.h"

int my_cd(int ac, char *av[])
{
	char * dir;
	int t;
		// traiter les arguments
	if (ac < 2){
		dir = getenv("HOME");
		if (dir == 0)
			dir = "/tmp";
	} else if (ac > 2){
		fprintf(stderr, "usage: %s [dir]\n", av[0]);
		return (-1);
	} else
		dir = av[1];

		// faire le boulot
	t = chdir(dir);

	if (t < 0){
		perror(dir);
		return (-1);
	}

	return (0);
}

int my_exit(int a, char *path[])
{
	printf("Aborted (core dumped)\n"); 
	exit(EXIT_SUCCESS);
}

int command_empty(int a, char *path[]) // fonction vide qui sert juste a retourner -2 pour le if ((*commandes[fd_Commande(mot[0])])(t,mot) == -2) de la commande execCommande
{
	return (-2);
}