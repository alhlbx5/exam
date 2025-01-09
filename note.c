





























#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
void	cpy(char *s, int fd)
{
	int i;
	i = 0;

	while(s && *(s + i))
	{
		write(fd, s + i, 1);
		i++;
	}
}
void	error(void)
{
	cpy("error: fatal\n", 2);
	exit(1);
}
int	ft_cd(char **av)
{
	int	i;
	i = 0;
	while(av[i])
		i++;

	if (i != 2)
	{
		cpy("error: cd: bad arguments\n", 2);
		return (1);
	}

	else if (chdir(av[1]) < 0)
	{
		cpy("error: cd: cannot change directory to ", 2);
		cpy(av[1], 2);
		cpy("\n", 2);
		return (1);
	}

	return (0);
}
int	ip(char **av)
{
	int	i;
	i = 0;

	while (av[i])
	{
		if (!strncmp(av[i], "|", 2))
		{
			av[i] = NULL;
			return (1);
		}

		else if (!strncmp(av[i], ";", 2))
		{
			av[i] = NULL;
			return (0);
		}
		i++;
	}

	return (0);
}
void	ex(int argc, char **av, char **env, int pi, int *pf)
{
	int		np;
	int		nf[2];
	pid_t	pid;
	int		status;
	int		i;
	
	if (argc <= 0)
		return ;
    
	np = ip(av);

	if (av[0])
	{
		if (np && pipe(nf) < 0)
			error();

		if (!strncmp(av[0], "cd", 3))
			ft_cd(av);

		else
		{
			pid = fork();
			if (pid < 0)
				error();
			else if (pid == 0)
			{
				if (pi && dup2(pf[0], 0) < 0)
					error();

				if (np && dup2(nf[1], 1) < 0)
					error();
    
				execve(av[0], av, env);
				cpy("error: cannot exute ", 2);
				cpy(av[0], 2);
				cpy("\n", 2);
				exit(1);
			}

			else
			{
				if (waitpid(pid, &status, 0) < 0)
					error();
        
				if (status / 256 == 255)
					error();
        
				if (pi)
					close(pf[0]);
    
				if (np)
					close(nf[1]);
			}
		}
	}
	// recursive call
	i = 0;

	while (av[i])
		i++;

	ex(argc - i - 1, av + i + 1, env, np, nf);
}
int	main(int argc, char **av, char **env)
{
	int	pi;
	int	pf[2];
	pi = 0;
	pf[0] = 0;
	pf[1] = 1;
	ex(argc - 1, av + 1, env, pi, pf);
	return (0);
}
/*


























































































































































*/