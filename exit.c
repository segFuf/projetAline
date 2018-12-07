#include "minishell.h"

int my_exit(int a, char *path[])
{
	printf("Aborted (core dumped)\n"); 
	exit(EXIT_SUCCESS);
}