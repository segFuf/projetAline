/* decouper.c
 Un wrapper autour de strtok
*/
#include "minishell.h"

/* decouper  --  decouper une chaine en mots */
int decouper(char * ligne, char * separ, char * mot[], int maxmot)
{
	int i;

	mot[0] = strtok(ligne, separ);
	for(i = 1; mot[i - 1] != 0; i++){
		if (i == maxmot){
			fprintf(stderr, "Erreur dans la fonction decouper: trop de mots\n");
			mot[i - 1] = 0;
		}
	mot[i] = strtok(NULL, separ);
	}
	return (i);
}

int findChaine(char *str) // cette fonction regarde dans la string str si il y a "&&"
{
	char test[3]; // pour me balader dans str 

	if (strlen(str) <= 2) // si la taille de str n'est pas bonne. il ne peut pas y avoir de "&&" si elle de fait que 2 de taille
		return (0);
	for (int i = 0; str[i + 1] != '\0'; i++) {
		test[0] = str[i];
		test[1] = str[i + 1]; // je prend 2 charactere de str et je teste avec la fonction strcmp
		if (strcmp(test,"&&") == 0) // strcmp renvoie 0 quand les string sont identique
			return (1);
	}
	return (0); 
}

int fd_Commande(char *str) // check si la commande existe
{
	if (strcmp(str,"exit") == 0)
		return (0); // si la commande est exit, renvoie 0 qui est la place de exit dans mon tableau de fonction
	if (strcmp(str,"cd") == 0)
		return (1);// si la commande est cd, renvoie 1 qui est la place de cd dans mon tableau de fonction

	return (2); // la valeur de retour 2 represente la fonction empty dans mon tableau de fonction car je n'ai pas fait la commande dans str pour l'instant
}

int (**get_tab_commande(void))(int , char *[]) // la fonction qui creer et renvoie le tableau de fonctions que j'utilise dans la fonction execCommande
{
	int (**tab)(int ,char *[]);

	tab = malloc(sizeof(*tab)*3);
	tab[0] = &my_exit; // quand fd_commande retourne 0, cela appellera my_exit. 
	tab[1] = &my_cd;
	tab[2] = &command_empty;

	return (tab);
}

#ifdef TEST 
int main(int ac, char * av[])
{
	char *elem[10];
	int i;

	if (ac < 3) {
		fprintf(stderr, "usage: %s phrase separ\n", av[0]);
		return 1;
	}

	printf("On decoupe '%s' suivant les '%s' :\n", av[1], av[2]);
	decouper(av[1], av[2], elem, 10);

	for(i = 0; elem[i] != 0; i++)
		printf("%d : %s\n", i, elem[i]);

	return 0;
}
#endif