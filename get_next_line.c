#include "get_next_line.h"

int	ft_strlen(const char *str)
{
	int i;

	if (!str)
	{
		return(0);
	}
	i = 0;
	while (str[i])
	{
		i++;
	}
	return(i);
}

char	*ft_strchr(char *str, int c)
{
	int i;

	if (!str)
	{
		return(NULL);
	}
	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
		{
			return(&str[i]);
		}
		i++;
	}
	return(NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char *join;
	int i;
	int j;

	if (!s1 || !s2)
	{
		return(NULL);
	}
	join = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) +1));
	if (!join)
	{
		return(NULL);
	}
	i = 0;
	while (s1 && s1[i])
	{
		join[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		join[i] =s2[j];
		i++;
		j++;
	}
	join[i] = '\0';
	free(s1);
	return(join);
}

char	*ft_strdup(char *src)
{
	char *dest;
	int i;

	if (!src)
	{
		return(NULL);
	}
	dest= malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (!dest)
	{
		return(NULL);
	}
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return(dest);
}

char	*ft_strcpy(char *dest, char *src)
{
	int i;

	if (!src)
	{
		return(NULL);
	}
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return(dest);
}

char	*get_next_line(int fd)
{
	static char buffer[BUFFER_SIZE +1];
	char *line;
	char *newline;
	int countread;


	line = ft_strdup(buffer);
	while (!(newline = ft_strchr(line, '\n')) && (countread =read(fd,buffer,BUFFER_SIZE )))
	{
		buffer[countread] = '\0';
		line = ft_strjoin(line, buffer);
	}
	if (ft_strlen(line) == 0)
	{
		return(free(line),NULL);
	}
	if (newline)
	{
		ft_strcpy(buffer, newline +1);
		*(newline +1)= '\0';
	}
	else
		buffer[0] = '\0';
	return(line);
}
int main(void)
{
	int i;
	int fd;

	fd = open("test.txt", O_RDONLY, 0644);
	if (fd == -1)
	{
		return(-1);
	}
	i = 0;
	while (i < 5)
	{
		printf("gn:%s", get_next_line(fd));
		i++;
	}
	close(fd);
	return(0);
}