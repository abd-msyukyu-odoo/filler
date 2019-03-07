/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:43:51 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/07 19:55:57 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	ystr_realloc(char **in, size_t old, size_t new)
{
	char		*out;

	if (new < old || (*in == NULL && old != 0))
		return (0);
	out = (char*)malloc(new + 1);
	if (!out)
		return (0);
	while (old-- > 0)
		out[old] = (*in)[old];
	out[new] = '\0';
	free(*in);
	*in = out;
	return (1);
}

static ssize_t			ystr_join(char **des, size_t ld, char *src, size_t ls)
{
	size_t		tmp;

	tmp = ls;
	if (!ystr_realloc(des, ld, ld + ls))
		return (-1);
	while (ls-- > 0)
		(*des)[ld + ls] = src[ls];
	return (ld + tmp);
}

static size_t			ystr_len(char *in)
{
	size_t		i;

	if (!in)
		return (0);
	i = 0;
	while (in[i] != '\0')
		i++;
	return (i);
}

static void				ysave_rmn(char *out, char *rmn, size_t len)
{
	size_t		i;

	i = 0;
	while (out[len + i] != '\0' && len + i < BUFF_SIZE)
	{
		rmn[i] = out[len + i];
		i++;
	}
}

static size_t			yassign(char *out)
{
	unsigned char	f;
	size_t			i;

	f = 0;
	i = 0;
	while (out[i] != '\0' && out[i] != 'P')
	{
		if (out[i] == 'p')
			f = 1;
		i++;
	}
	return ((f) ? i : 0);
}

static size_t			yturn(char *out)
{
	//chercher l'espace apres PLATEAU
	//faire atouilimited
	//incrementer de 2
	//passer tous les x \n suivants
	//chercher l'espace apres PIECE
	//faire atouilimited
	//incrementer de 1
	//passer tous les \n suivants
	
}

static unsigned char	ycomplete(char *out, char *rmn)
{
	size_t		len;

	if (len = yassign(out))
	{
		ysave_rmn(out, rmn, len);
		return (1);
	}
	if (len = yturn(out))
	{
		ysave_rmn(out, rmn, len);
		return (1);
	}
	return (0);
}

char					*yread_input()
{
	char			buff[BUFF_SIZE + 1];
	ssize_t			nread;
	char			*out;
	ssize_t			len;
	static char		rmn[BUFF_SIZE + 1] = {0};

	len = 0;
	out = NULL;
	read = ystr_len(rmn);
	ystr_join(&out, len, rmn, read);
	len = read;
	while ((nread = read(INPUT, buff, BUFF_SIZE)))
	{
		if (nread < 0 || -1 == (len = ystr_join(&out, len, buff, nread)))
			return (NULL);
		if (ycomplete(&out, &rmn))
			return (out);
	}
	return (out);
}
