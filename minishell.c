#include "minishell.h"

enum {
  MaxLigne = 1024,              // longueur max d'une ligne de commandes
  MaxMot = MaxLigne / 2,        // nbre max de mot dans la ligne
  MaxDirs = 100,		// nbre max de repertoire dans PATH
  MaxPathLength = 512,		// longueur max d'un nom de fichier
  MaxCommandes = MaxMot / 2,
};

int command_empty(int a, char *path[]) // fonction vide qui sert juste a retourner -2 pour le if ((*commandes[fd_Commande(mot[0])])(t,mot) == -2) de la commande execCommande
{
	return (-2);
}

void execCommande(char *dirs[], char *mot[], int t)
{
	char pathname[MaxPathLength];
	int (**commandes)(int ,char *[]); // création d'un pointeur sur pointeur de fonction

	commandes = get_tab_commande(); // je recupére le tableau de fonction
	if ( (*commandes[fd_Commande(mot[0])])(t,mot) == -2) { // appelle de la fonction du tableau de fonction, fd_commande doit renvoyer l'endroit ou se trouve la commande demandé dans le tableau
		for(int i = 0; dirs[i] != 0; i++) {  // si je n'ai pas fait la commande demandé, fd_fonction va renvoyer 2 qui est la place de command_empty qui renvoie -2. On execute alors avec exec.
			snprintf(pathname, sizeof pathname, "%s/%s", dirs[i], mot[0]);
			execv(pathname, mot);
		}
					// aucun exec n'a fonctionne
		fprintf(stderr, "%s: not found\n", mot[0]);
		exit(1);
	}
}

int execBasic(char *commandes, char *mot[], char *dirs[]) // fonction qui execute un commande sans gerer &&
{
	int tmp, nb_arg, multi;
	//int multi = 1;

	if (commandes[strlen(commandes) -2] == '&') { // test si il souhaite la faire en arriere plan
		commandes[strlen(commandes) -2] = 0; // je supprime le & de la chaine de charatere pour eviter les erreurs
		multi = 0;
	}
	nb_arg = decouper(commandes, " \t\n", mot, MaxMot);
	if (mot[0] == 0)	// ligne vide
		return (0);
	if (fd_Commande(mot[0]) >= 2 || multi == 0) { // si c'est cd ou exit, pas de fork sauf si il veut le faire en arriere plan avec &, bah le cd ou exit marchera pas mais il fait ce qu'il veut
		tmp = fork();		// lancer le processus enfant
		if (tmp < 0) {
			perror("fork");
			return (0);
		}

		if (tmp != 0) {		// parent : attendre la fin de l'enfant
			if (multi != 0) // multi = 0 si on a demander a le faire en arriere plan donc on wait pas le fils dans ce cas
				while(wait(0) != tmp);
			else
				printf("pid : %d\n",tmp);
			return (0);
		}
					// enfant : exec du programme
	}
	execCommande(dirs, mot, nb_arg - 1);
	return (0);
}

int execChaine(char *commandes, char *mot[], char *dirs[]) // fonction qui exectue une commande dans le cas ou il y a un ou plusieur &&
{
	char *suite[MaxCommandes];
	int sui, tmp, status, nb_arg, fuf;

	if (commandes[strlen(commandes) -2] == '&') { // test si il souhaite la faire en arriere plan
		commandes[strlen(commandes) -2] = 0;
		fuf = fork(); // dans le cas d'une commande avec &&, le parent fork et se casse pour laisser le fils faire toutes les commandes en arriere plan
		if (fuf)
			return (1);
	}	
	sui = decouper(commandes, "&&", suite, MaxCommandes); // je decoupe la chaine de char en mot contenant les commande entre les &&
	for (int y = 0; sui >= 2; y++, sui--) { // je boucle pour exectuer toutes les commandes
		nb_arg = decouper(suite[y], " \t\n", mot, MaxMot);
		if (mot[0] == 0)	// ligne vide
			continue;
		if (fd_Commande(mot[0]) >= 2) { // si c'est cd ou exit, pas de fork sauf si il veut le faire en arriere plan avec &, bah le cd ou exit marchera pas mais il fait ce qu'il veut
			tmp = fork();		// lancer le processus enfant
			if (tmp < 0) {
				perror("fork");
				continue;
			}

			if (tmp != 0) {		// parent : attendre la fin de l'enfant
				while(wait(&status) != tmp);
				if (WEXITSTATUS(status) == 1) // exit(1) dans la fonction execCommande n'est appeller que quand il y a eu une erreur
					return (1); // on se casse
				continue;
			}
		}			// enfant : exec du programme
		execCommande(dirs, mot, nb_arg - 1);
	}
	if (tmp > 0) // dans le cas ou c'etait en arriere plan avec "&" le fils doit s'arreter sinon sa va faire buger le shell
		exit(1);
	return (0);
}

void Prompt() // affiche le chemin pour le prompt
{
	char *str;
	char *path = malloc(sizeof(*path) * 1021); 
	str = strcat(getcwd(path,1024), " C:\\ "); // best prompt ever
	printf("%s",str);
	free(path);
}

int main(int argc, char * argv[])
{
	char ligne[MaxLigne];
	char *mot[MaxMot];
	char *commandes[MaxCommandes];
	char *dirs[MaxDirs];
	int i, com;

	/* Decouper PATH en repertoires */
	decouper(getenv("PATH"), ":", dirs, MaxDirs);

	/* Lire et traiter chaque ligne de commande */
	for(Prompt(); fgets(ligne, sizeof ligne, stdin) != 0; Prompt()) {
		com = decouper(ligne, ";", commandes, MaxCommandes); // je decoupe ligne en mot délimiter par ";" 
		for (i = 0; com >= 2; i++, com--) { // pour chaque mot
			if (findChaine(commandes[i])) // si il y a un && dedans
				execChaine(commandes[i], mot, dirs);
			else // sinon
				execBasic(commandes[i], mot, dirs);
		}
	}
	return 0;
}