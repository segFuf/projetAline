#include "minishell.h"

void execCommande(char *mot[], int t, env_t *env)
{
	char pathname[MaxPathLength];
	int (**commandes)(int ,char *[]); // création d'un pointeur sur pointeur de fonction

	set_redirections(mot,t,env);
	set_std_red(mot,t,env);
	commandes = get_tab_commande(); // je recupére le tableau de fonction
	if ( (*commandes[fd_Commande(mot[0])])(t,mot) == -2) { // appelle de la fonction du tableau de fonction, fd_commande doit renvoyer l'endroit ou se trouve la commande demandé dans le tableau
		for(int i = 0; env->dirs[i] != 0; i++) {  // si je n'ai pas fait la commande demandé, fd_fonction va renvoyer 2 qui est la place de command_empty qui renvoie -2. On execute alors avec exec.
			snprintf(pathname, sizeof pathname, "%s/%s", env->dirs[i], mot[0]);
			execv(pathname, mot);
		}
					// aucun exec n'a fonctionne
		fprintf(stderr, "%s: not found\n", mot[0]);
		exit(1);
	}
}

int execution(char *commandes, env_t *env, int multi)
{
	char *mot[MaxMot];
	int nb_arg, tmp, status;

	nb_arg = decouper(commandes, " \t\n", mot, MaxMot);
	if (mot[0] == 0)	// ligne vide
		return (0);
	if (fd_Commande(mot[0]) >= 2 || multi == 1) { // si c'est cd ou exit, pas de fork sauf si il veut le faire en arriere plan avec &, bah le cd ou exit marchera pas mais il fait ce qu'il veut
		tmp = fork();		// lancer le processus enfant
		if (tmp < 0) {
			perror("fork");
			return (0);
		}
		if (tmp != 0) {		// parent : attendre la fin de l'enfant
			if (env->in[0] != 0)
				close(env->in[1]);
			if (env->out[0] != 0)
				close(env->out[1]);
			if (multi == 0) { // multi = 0 si on a demander a le faire en arriere plan donc on wait pas le fils dans ce cas
				while(wait(&status) != tmp);
				if (WEXITSTATUS(status) == 1) // exit(1) dans la fonction execCommande n'est appeller que quand il y a eu une erreur
					return (0); // on se casse
			} 
			else
				printf("pid : %d\n",tmp);
			return (1);
		}
	}
	if (env->in[0] != 0) {
		dup2(env->in[0], 0);
		close(env->in[1]);
	}
	if (env->out[0] != 0)
		dup2(env->out[1], 1);
	execCommande(mot, nb_arg - 1, env);
	return (0);
}

int execPipe(char *commandes, int multi, env_t *env)
{
	char *pipes[MaxCommandes];
	int pi;
	pi = decouper(commandes, "|", pipes, MaxCommandes); // je decoupe la chaine de char en mot contenant les commande entre les |
	for (int y = 0; pi >= 2; y++, pi--) { // je boucle pour exectuer toutes les commandes
		if (pi == 2) {
			env->in[0] = dup(env->out[0]);
			env->in[1] = dup(env->out[1]);
			env->out[0] = 0;
		} else if (env->out[0] != 0) {
			env->in[0] = dup(env->out[0]);
			env->in[1] = dup(env->out[1]);
			pipe(env->out);
		} else if ((env->in[0] == 0) && (env->out[0] == 0)) {
			pipe(env->out);
		}
		if (execution(pipes[y], env, multi) == 0)
			return (0);
	}
	return (1);
}

int execBasic(char *commandes, env_t *env) // fonction qui execute un commande sans gerer &&
{
	int multi = 0;

	if (commandes[strlen(commandes) -2] == '&') { // test si il souhaite la faire en arriere plan
		commandes[strlen(commandes) -2] = 0; // je supprime le & de la chaine de charatere pour eviter les erreurs
		multi = 1;
	}	
	if (findPipe(commandes)) {
			execPipe(commandes, multi, env);
			clear_pipe(env);
	} else
		execution(commandes, env, multi);
}

int execChaine(char *commandes, env_t *env) // fonction qui exectue une commande dans le cas ou il y a un ou plusieur &&
{
	char *suite[MaxCommandes];
	int sui, fuf = 1;

	if (commandes[strlen(commandes) -2] == '&') { // test si il souhaite la faire en arriere plan
		commandes[strlen(commandes) -2] = 0;
		fuf = fork(); // dans le cas d'une commande avec &&, le parent fork et se casse pour laisser le fils faire toutes les commandes en arriere plan
		if (fuf) {
			printf("pid : %d\n",fuf);
			return (1);
		}
	}	
	sui = decouper(commandes, "&&", suite, MaxCommandes); // je decoupe la chaine de char en mot contenant les commande entre les &&
	for (int y = 0; sui >= 2; y++, sui--) { // je boucle pour exectuer toutes les commandes
		if (findPipe(suite[y])) {
			if (execPipe(suite[y],0,env)) {
				clear_pipe(env);
				return (0);
			}
			clear_pipe(env);
		} else {
			if (execution(suite[y], env, 0) == 0)
				return (0);
		}
	}
	if (fuf == 0) // dans le cas ou c'etait en arriere plan avec "&" le fils doit s'arreter sinon sa va faire buger le shell
		exit(1);
	return (0);
}