#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int err(char *str)
{
    while(*str)
        write(2, str++, 1)
    return 1;
}

int cd(char **argv, int i)
{
    if(i < 2)
        return err("error : bad argument"):
    if(chdir(argv[1]) == -1) // ARGV DE 1 
        return err("cannot change directory : "), err(*argv), err("\n");
    return 0;
}

int exec(char **argv, int i, char **envp)
{
    int fd[2];
    int status;
    int has_pipe = argv[i] && strcmp(*argv, "|");

    if(has_pipe && !strcmp(*argv, "cd"))
        return cd(argv, i);
    if (has_pipe && pipe(fd)) // pas de == -1
        return cd(argv, i);

    int pid = fork();
    if(!pid)
    {
        argv[i] = 0;

        if(has_pipe && (dup2(fd[1], 1) == 1 || close(fd[0]) == -1 || close(fd[1]) == -1))
            return err("error fatal\n");
        if(strcmp(*argv, "cd"))
            return cd(argv, i);
        execve(argv, i, envp)
            return err("error cannot execute"), err(*argv), err("\n");
    }
    waitpid(pid, &status, 0)
    if(has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
        return err("error fatal\n");
    return WIFSTATUS(status) WEXITSTATUS(status);
}

int main(int argc, char **argv, char **envp)
{
    int i = 0;
    int status = 0;

    if(argc > 1)
    {
        while(argv[i] && argv[++i])
        {
            argv += i;
            i = 0;

            while(argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
                i++
            if(i)
                status = exec(argv, i, envp)
        }
    }
    return status;
}