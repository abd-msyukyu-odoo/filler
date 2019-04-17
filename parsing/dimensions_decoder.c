/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dimensions_decoder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 12:27:06 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 12:27:07 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

unsigned char			ydecode_size(t_in *in, int *w, int *h, char *ref)
{
	size_t				p;

	p = 0;
	while (in->s[p + in->p] == ref[p])
		p++;
	if (ref[p] != '\0')
		return (0);
	in->p += p;
	*h = yatoui_limited(in);
	if (in->s[in->p++] != ' ')
		return (0);
	*w = yatoui_limited(in);
	if (in->s[in->p++] != ':')
		return (0);
	if (in->s[in->p++] != '\n')
		return (0);
	return (1);
}

ssize_t					ymap_read_size(t_map *map)
{
	int				i;
	size_t			ref;
	int				info;
	ssize_t			res;

	ref = 1000;
	i = 0;
	info = 3;
	res = 0;
	while (i < map->h)
	{
		if ((size_t)i == ref)
		{
			info++;
			ref *= 10;
		}
		res += (ssize_t)info;
		i++;
	}
	return ((ssize_t)map->h * (2 + (ssize_t)map->w) + res);
}
