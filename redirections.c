#include "minishell.h"

int redirec(char *mot[], int nb_arg, int *file)
{
	char **redictions = malloc(sizeof(*redictions) * 6);
	redictions[0] = ">";
	redictions[1] = ">>";
	redictions[2] = "2>";
	redictions[3] = "2>>";
	redictions[4] = "&>";
	redictions[5] = "&>>";

	for (int i = 1; i < nb_arg; i++) {
		for (int a = 0, y = 2; a < 6; a+=2, y *= 2) {
			if ((strcmp(mot[i],redictions[a]) == 0) || 
			(strcmp(mot[i],redictions[a + 1]) == 0)) {
				if (strstr(mot[i],">>") != NULL)
					y++;
				*file = i + 1;
				free(redictions);
				return (y);
			}
		}
	}
	free(redictions);
	return (0);
}

int set_redirections(char *mot[], int nb_arg, env_t *env)
{
	int a, fd, file;
	if ((a = redirec(mot, nb_arg, &file)) == 0)
		return (0);
		
	if (a & 1) {
		fd = open(mot[file], O_WRONLY | O_CREAT);
		lseek(fd,0,SEEK_END);
		a--;
	} else
		fd = open(mot[file], O_WRONLY | O_CREAT | O_TRUNC);

	switch (a) {
		case 2 : 
			dup2(fd,env->fd[out]);
			break;
		case 4 : 
			dup2(fd,env->fd[err]);
			break;
		case 8 : 
			dup2(fd,env->fd[out]);
			dup2(fd,env->fd[err]);

			break;
	}
	mot[file - 1] = mot[file + 1];
	return (1);
}

/*
2>&1 redirige stderr dans stdout 					stderr > stdout
1>&2 redirige stdout dans stderr					stdout > stderr
*/

int set_double_red(char *mot[], int nb_arg, env_t *env)
{
	char **redictions = malloc(sizeof(*redictions) * 3);
	redictions[0] = "2>&1";
	redictions[1] = "1>&2";
	redictions[2] = NULL;

	for (int i = 0; i < nb_arg && mot[i] != NULL; i++) {
		//printf("mot : %s\n",mot[i]);
		for (int a = 0; a < 2; a++) {
			if (strcmp(mot[i],redictions[a]) == 0) {
				//printf("fderr : %d fdout : %d\n",env->fd[err],env->fd[out]);
				if (a)
					dup2(env->fd[err], env->fd[out]);
				else  
					dup2(env->fd[out], env->fd[err]);
				//printf("fderr : %d fdout : %d\n",env->fd[err],env->fd[out]);
				mot[i] = NULL;
				free(redictions);
				return (1);
			}
		}
	}
	free(redictions);
	return (0);
}