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