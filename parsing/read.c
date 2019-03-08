/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:43:51 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/08 15:08:40 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	ystr_realloc(char **in, ssize_t old, ssize_t new)
{
	char		*out;

	if (old < 0 || new < 0 || new < old || (*in == NULL && old != 0))
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

static ssize_t			ystr_join(char **des, ssize_t ld, char *src, ssize_t ls)
{
	ssize_t		tmp;

	tmp = ls;
	if (ld < 0 || ls < 0 || !src || !ystr_realloc(des, ld, ld + ls))
		return (-1);
	while (ls-- > 0)
		(*des)[ld + ls] = src[ls];
	return (ld + tmp);
}

static char				*yread_input(char *out, ssize_t *len)
{
	char			buff[BUFF_SIZE + 1];
	ssize_t			nread;

	while ((nread = read(INPUT, buff, BUFF_SIZE)))
	{
		if (nread < 0 || -1 == (len = ystr_join(&out, len, buff, nread)))
		{
			free(out);
			return (NULL);
		}
	}
	return (out);
}

static char				*yempty_until(char *out, ssize_t *len, char *rmn,
		char end)
{
	ssize_t			i;
	ssize_t			j;

	i = 0;
	while (rmn[i] != '\0' && rmn[i] != end)
		i++;
	i = (rmn[i] == end) ? i + 1 : i;
	if (-1 == (*len = ystr_join(&out, *len, rmn, i)))
	{
		free(out);
		return (NULL);
	}
	j = i;
	while (j < BUFF_SIZE + 1)
	{
		rmn[j - i] = rmn[j];
		j++;
	}
	return (out);
}

static unsigned char	yfind_end(t_fe fe, ssize_t plen, ssize_t *len,
		char end)
{
	ssize_t			i;

	i = plen;
	while (i < *len && fe.out[i] != )
	{

		i++;
	}
}

static char				*yread_until(char end)
{
	static char		rmn[BUFF_SIZE + 1] = {0};
	ssize_t			len;
	char			*out;
	ssize_t			plen;
	t_fe			fe;

	out = NULL;
	out = yempty_until(out, &len, rmn, end);
	if (!out)
		return (NULL);
	plen = len;
	while ((out = yread_input(out, &len)))
	{
		fe = (t_fe){out, rmn};
		if (yfind_end(fe, plen, &len, end))
			return (out);
		plen = len;
	}
	return (out);
}

char					*yread_input(ssize_t n, char *end)
{
	if (end)
		return (yread_until(*end));
	return (yread_n(n));
}
