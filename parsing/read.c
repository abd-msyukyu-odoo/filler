/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:43:51 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/13 13:56:14 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	ystr_realloc(char **in, ssize_t old, ssize_t new,
	ssize_t l)
{
	char		*out;

	if (old < 0 || new < 0 || new < old || (*in == NULL && old != 0))
		return (0);
	out = (char*)malloc(l);
	if (!out)
		return (0);
	while (old-- > 0)
		out[old] = (*in)[old];
	out[new] = '\0';
	free(*in);
	*in = out;
	return (1);
}

static unsigned char	ystr_over_realloc(t_string *in, ssize_t old,
	ssize_t new)
{
	if (old < 0 || new < 0 || new < old || (in->s == NULL && old != 0))
		return (0);
	if (in->l >= new + 1)
	{
		(in->s)[new] = '\0';
		return (1);
	}
	if (in->l <= 0)
		in->l = 2;
	while (in->l < new + 1)
		in->l *= 2;
	return (ystr_realloc(&(in->s), old, new, in->l));
}

static ssize_t			ystr_join(t_string *des, ssize_t ld, char *src,
	ssize_t ls)
{
	ssize_t		tmp;

	tmp = ls;
	if (ld < 0 || ls < 0 || !src || !ystr_over_realloc(des, ld, ld + ls))
		return (-1);
	while (ls-- > 0)
		(des->s)[ld + ls] = src[ls];
	return (ld + tmp);
}

static unsigned char	yread_input(t_string *out, ssize_t *len, size_t sread)
{
	static char		buff[MAX_BUFF + 1];
	ssize_t			nread;

	if (sread >= MAX_BUFF)
		sread = MAX_BUFF;
	if ((nread = read(INPUT, buff, sread)))
	{
		if (nread < 0 || -1 == (*len = ystr_join(out, *len, buff, nread)))
		{
			free(out->s);
			return (0);
		}
	}
	return (1);
}

static unsigned char	yempty_until(t_string *out, ssize_t *len, char *rmn,
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
		free(out->s);
		out->s = NULL;
		return (0);
	}
	j = i;
	while (rmn[j] != '\0')
	{
		rmn[j - i] = rmn[j];
		j++;
	}
	rmn[j - i] = '\0';
	return (*len != 0 && ((out->s)[*len - 1] == end) ? 1 : 0);
}

static unsigned char	yempty_n(t_string *out, ssize_t *len, char *rmn,
	ssize_t n)
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
		free(out->s);
		out->s = NULL;
		return (0);
	}
	j = i;
	while (rmn[j] != '\0')
	{
		rmn[j - i] = rmn[j];
		j++;
	}
	rmn[j - i] = '\0';
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
		fe.rmn[j] = '\0';
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
	fe.rmn[i] = '\0';
	*len = n;
	return (1);
}

static char				*yread_until(char end, char *rmn)
{
	ssize_t			len;
	t_string		out;
	ssize_t			plen;
	t_fe			fe;
	size_t			sread;

	out.s = NULL;
	out.l = 0;
	len = 0;
	if (yempty_until(&out, &len, rmn, end))
		return (out.s);
	if (!out.s)
		return (NULL);
	plen = len;
	sread = BUFF_SIZE;
	while (yread_input(&out, &len, sread))
	{
		fe = (t_fe){out.s, rmn};
		if (yfind_end(fe, plen, &len, end))
			return (out.s);
		plen = len;
		sread = (2 * sread < MAX_BUFF) ? 2 * sread : sread;
	}
	return (out.s);
}

static char				*yread_n(ssize_t n, char *rmn)
{
	ssize_t			len;
	t_string		out;
	t_fe			fe;
	size_t			sread;

	out.s = NULL;
	out.l = 0;
	len = 0;
	if (yempty_n(&out, &len, rmn, n))
		return (out.s);
	if (!out.s)
		return (NULL);
	sread = BUFF_SIZE;
	while (yread_input(&out, &len, sread))
	{
		fe = (t_fe){out.s, rmn};
		if (yfind_n(fe, &len, n))
			return (out.s);
		sread = (2 * sread < MAX_BUFF) ? 2 * sread : sread;
	}
	return (out.s);
}

char					*yread(ssize_t n, char *end)
{
	static char		rmn[MAX_BUFF + 1];

	if (end != NULL)
		return (yread_until(*end, rmn));
	if (n > 0)
		return (yread_n(n, rmn));
	return (NULL);
}
