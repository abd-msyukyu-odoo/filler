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

char					*yread_input()
{
	char			buff[BUFF_SIZE + 1];
	ssize_t			nread;
	char			*out;
	ssize_t			len;

	len = 0;
	out = NULL;
	while ((nread = read(INPUT, buff, BUFF_SIZE)))
	{
		if (nread < 0 || -1 == (len = ystr_join(&out, len, buff, nread)))
			return (NULL);
	}
	return (out);
}
