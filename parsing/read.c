/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:43:51 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/11 16:56:12 by dabeloos         ###   ########.fr       */
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

	if ((nread = read(INPUT, buff, BUFF_SIZE)))
	{
		if (nread < 0 || -1 == (*len = ystr_join(&out, *len, buff, nread)))
		{
			free(out);
			return (NULL);
		}
	}
	return (out);
}

static unsigned char	yempty_until(char **out, ssize_t *len, char *rmn,
		char end)
{
	ssize_t			i;
	ssize_t			j;

	i = 0;
	while (rmn[i] != '\0' && rmn[i] != end)
		i++;
	i = (rmn[i] == end) ? i + 1 : i;
	if (-1 == (*len = ystr_join(out, *len, rmn, i)))
	{
		free(*out);
		*out = NULL;
		return (0);
	}
	j = i;
	while (j < BUFF_SIZE + 1)
	{
		rmn[j - i] = rmn[j];
		j++;
	}
	return (((*out)[*len - 1] == end) ? 1 : 0);
}

static unsigned char	yempty_n(char **out, ssize_t *len, char *rmn, ssize_t n)
{
	ssize_t			i;
	ssize_t			j;

	if (*len >= n)
		return (1);
	i = 0;
	while (rmn[i] != '\0' && *len + i < n)
		i++;
	if (-1 == (*len = ystr_join(out, *len, rmn, i)))
	{
		free(*out);
		*out = NULL;
		return (0);
	}
	j = i;
	while (j < BUFF_SIZE + 1)
	{
		rmn[j - i] = rmn[j];
		j++;
	}
	return ((*len == n) ? 1 : 0);
}

static unsigned char	yfind_end(t_fe fe, ssize_t plen, ssize_t *len,
		char end)
{
	ssize_t			i;
	ssize_t			j;

	i = plen;
	while (i < *len && fe.out[i] != end)
		i++;
	if (fe.out[i++] == end)
	{
		j = 0;
		while (i + j < *len)
		{
			fe.rmn[j] = fe.out[i + j];
			j++;
		}
		fe.out[i] = '\0';
		*len = i;
		return (1);
	}
	return (0);
}

static unsigned char	yfind_n(t_fe fe, ssize_t *len, ssize_t n)
{
	ssize_t			i;

	if (*len < n)
		return (0);
	if (*len == n)
		return (1);
	i = 0;
	while (n + i < *len)
	{
		fe.rmn[i] = fe.out[n + i];
		i++;
	}
	fe.out[n] = '\0';
	*len = n;
	return (1);
}

static char				*yread_until(char end, char *rmn)
{
	ssize_t			len;
	char			*out;
	ssize_t			plen;
	t_fe			fe;

	out = NULL;
	len = 0;
	if (yempty_until(&out, &len, rmn, end))
		return (out);
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

static char				*yread_n(ssize_t n, char *rmn)
{
	ssize_t			len;
	char			*out;
	t_fe			fe;

	out = NULL;
	len = 0;
	if (yempty_n(&out, &len, rmn, n))
		return (out);
	if (!out)
		return (NULL);
	while ((out = yread_input(out, &len)))
	{
		fe = (t_fe){out, rmn};
		if (yfind_n(fe, &len, n))
			return (out);
	}
	return (out);
}

extern FILE *fd;
char					*yread(ssize_t n, char *end)
{
	static char		rmn[BUFF_SIZE + 1] = {0};
	
	fd = fopen("output_test.txt", "a");
	fprintf(fd, "read : \n");
	if (end)
	{
		char *o1 = yread_until(*end, rmn);
		fprintf(fd, "%s\n", o1);
		fclose(fd);
		return (o1);
	}
	if (n > 0)
	{
		char *o2 = yread_n(n, rmn);
		fprintf(fd, "%s\n", o2);
		fclose(fd);
		return (o2);
	}
	fclose(fd);
	return (NULL);
}
