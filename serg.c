#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
void	ft_putstr_fd(char *s, int fd)
{
	int i;
	i = 0;
	while(s && *(s + i))
	{
		write(fd, s + i, 1);
		i++;
	}
}
void	error_fatal(void)
{
	ft_putstr_fd("error: fatal\n", 2);
	exit(1);
}
int	ft_cd(char **argv)
{
	int	i;
	i = 0;
	while(argv[i])
		i++;
	if (i != 2)
	{
		ft_putstr_fd("error: cd: bad arguments\n", 2);
		return (1);
	}
	else if (chdir(argv[1]) < 0)
	{
		ft_putstr_fd("error: cd: cannot change directory to ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	return (0);
}
int	is_pipe(char **argv)
{
	int	i;
	i = 0;
	while (argv[i])
	{
		if (!strncmp(argv[i], "|", 2))
		{
			argv[i] = NULL;
			return (1);
		}
		else if (!strncmp(argv[i], ";", 2))
		{
			argv[i] = NULL;
			return (0);
		}
		i++;
	}
	return (0);
}
void	exec(int argc, char **argv, char **env, int prev_is_pipe, int *prev_fd)
{
	int		next_is_pipe;
	int		next_fd[2];
	pid_t	pid;
	int		status;
	int		i;
	
	if (argc <= 0)
		return ;
	next_is_pipe = is_pipe(argv);
	if (argv[0])
	{
		if (next_is_pipe && pipe(next_fd) < 0)
			error_fatal();
		if (!strncmp(argv[0], "cd", 3))
			ft_cd(argv);
		else
		{
			pid = fork();
			if (pid < 0)
				error_fatal();
			else if (pid == 0)
			{
				if (prev_is_pipe && dup2(prev_fd[0], 0) < 0)
					error_fatal();
				if (next_is_pipe && dup2(next_fd[1], 1) < 0)
					error_fatal();
				execve(argv[0], argv, env);
				ft_putstr_fd("error: cannot execute ", 2);
				ft_putstr_fd(argv[0], 2);
				ft_putstr_fd("\n", 2);
				exit(1);
			}
			else
			{
				if (waitpid(pid, &status, 0) < 0)
					error_fatal();
				if (status / 256 == 255)
					error_fatal();
				if (prev_is_pipe)
					close(prev_fd[0]);
				if (next_is_pipe)
					close(next_fd[1]);
			}
		}
	}
	// recursive call
	i = 0;
	while (argv[i])
		i++;
	exec(argc - i - 1, argv + i + 1, env, next_is_pipe, next_fd);
}
int	main(int argc, char **argv, char **env)
{
	int	prev_is_pipe;
	int	prev_fd[2];
	prev_is_pipe = 0;
	prev_fd[0] = 0;
	prev_fd[1] = 1;
	exec(argc - 1, argv + 1, env, prev_is_pipe, prev_fd);
	return (0);
}
/*


























































































































































*/