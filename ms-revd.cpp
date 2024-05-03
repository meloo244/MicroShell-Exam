#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int	err(char *str) // parametre (char étoile str)
{
	while (*str) // tant que étoile str
			write(2, str++, 1);
		return 1;
}

int cd(char **argv, int i)
{
	if (i != 2)
		return err("error cd: bad arguments\n");
	if (chdir(argv[1]) == -1)
		return err("error cd: cannot change directory to "), err(argv[1]), err("\n");
	return 0;
}

int exec(char **argv, int i)
{
	int fd[2];
	int status;
	int has_pipe = argv[i] && !strcmp(argv[i], "|");

	if(!has_pipe && !strcmp(*argv, "cd")) // verif cd
		return cd(argv, i);

	if(has_pipe && pipe(fd) == -1)
		return cd(argv, i);

	int pid = fork();
	if(!pid)
	{
		argv[i] = 0;				// milleux fd de 0 cote fd de 1
		if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return err("erreur fatal\n");
		if(!strncmp(*argv, "cd")) // verif si ce n'est pas cd
			return cd(argv, i);
		execve(*argv, argv, __environ);
		return err("error : cannot execute "), err(*argv), err("\n"); // je return l'erreur l'argv et le back slash n
	}

	waitpid(pid, &status, 0); // WAITPID : pid, adresse du status et le 0
	if (has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == - 1))
		return err("error \n");
	return WIFEXITED(status) && WEXITSTATUS(status); // WIF - WEXIT EXITED STATUS de status
}

int main(int argc, char **argv)
{
	int	i = 0;
	int status = 0;

	if(argc > 1)
	{
		while (argv[i] && argv[++i])
		{
			argv += i;
			i = 0;
			while (argv[i] && strcmp(arv[i], "|") && strcmp(argv[i], ";"))
				i++;
			if(i)
				status = exec(argv, i);
		}
	}
	return status;
}
