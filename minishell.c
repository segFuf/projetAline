#include "minishell.h"


void Prompt() // affiche le chemin pour le prompt
{
	char *str;
	char *path = malloc(sizeof(*path) * 1024); 
	str = strcat(getcwd(path,1021), " C:\\ "); // best prompt ever
	printf("%s",str);
	free(path);
}

env_t *set_env()
{
	env_t *new = malloc(sizeof(*new));

	new->fd[in] = fileno(stdin);
	new->fd[out] = fileno(stdout);
	new->fd[err] = fileno(stderr);

	new->in[0] = 0;
	new->out[0] = 0;

	/* Decouper PATH en repertoires */
	decouper(getenv("PATH"), ":", new->dirs, MaxDirs);

	return (new);
}

int main(int argc, char * argv[])
{
	env_t *env;
	char ligne[MaxLigne];
	char *commandes[MaxCommandes];
	int i, com;


	env = set_env();
	/* Lire et traiter chaque ligne de commande */
	for(Prompt(); fgets(ligne, sizeof ligne, stdin) != 0; Prompt()) {
		com = decouper(ligne, ";", commandes, MaxCommandes); // je decoupe ligne en mot délimiter par ";" 
		for (i = 0; com >= 2; i++, com--) { // pour chaque mot
			if (findChaine(commandes[i])) // si il y a un && dedans
				execChaine(commandes[i], env);
			else // sinon
				execBasic(commandes[i], env);
		}
	}
	return 0;
}