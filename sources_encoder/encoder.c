/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encoder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guribeir <guribeir@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 10:13:14 by guribeir          #+#    #+#             */
/*   Updated: 2023/01/13 19:08:16 by guribeir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "labs.h"

int share_info(char *str, size_t size)
{
	int		shmid;
	key_t	key;
	char	*shmp;

	key = ftok(".shmfile", 65);
	shmid = shmget(key, size, 0644|IPC_CREAT);
	if (shmid == -1)
	{
		perror("Shared memory");
		return (1);
	}
	shmp = shmat(shmid, NULL, 0);
	if (shmp == (void *) -1)
	{
		perror("Shared memot attach");
		return (1);
	}
	memmove(shmp, str, size);
	printf("--- String compartilhada --- %s\n", shmp);
	sleep(5);
	if (shmdt(shmp) == -1)
	{
		perror("shmdt");
		return 1;
	}
	if (shmctl(shmid, IPC_RMID, 0) == -1)
	{
		perror("shmctl");
		return 1;
	}
	return (0);
}

//share_info(str, strlen(str));

int	get_str_size(char **dict, char *str)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while(str[i])
	{
		size = size + strlen(dict[(int)(str[i])]);
		i++;
	}
	return (size + 1);
}

char	*encode(char **dict, char *str)
{
	char	*code;
	int		i;
	int		size;

	i = 0;
	size = get_str_size(dict, str);	
	code = calloc(size, sizeof(char)); // será q aqui n é size + 1?
	while(str[i])
	{
		strcat(code, dict[(int)(str[i])]);
		i++;
	}
	return (code);
}

char	*decode(char *str, char**dict)
{
	char	*new_str; //malloc nisso
	int		j;
	char	c[2];
	
	j = 0;
	new_str = NULL;
	new_str = calloc(strlen(str), sizeof(char));
	while(*str)
	{
		if (strlen(dict[j]) > 0)
		{
			if(strncmp(dict[j], (str), strlen(dict[j])) == 0)//pode ser q aqui faça a ultima leitura de maneira inesperada/incorreta
			{
				c[0] = j;
				c[1] = '\0';
				strncat(new_str, c, 1);
				str += strlen(dict[j]);
				j = -1;
			}
		}
		j++;
	}
	return (new_str);
}