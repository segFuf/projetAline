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
	new->commandes = get_tab_commande(); // je recupére le tableau de fonction
	

	/* Decouper PATH en repertoires */
	decouper(getenv("PATH"), ":", new->dirs, MaxDirs);

	return (new);
}

/*char *get_line(char *line, int size, FILE *stream, int *hist, env_t *env)
{
	int c;
	while (c != 10) {
		c = get
	}

}*/

int main(void)
{
	env_t *env;
	char ligne[MaxLigne];
	char *commandes[MaxCommandes];
	int i, com, *hist;


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