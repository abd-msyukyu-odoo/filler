/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_decoder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 12:20:34 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 12:20:36 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	ydecode_line_map(t_in *in, t_map *map, int y)
{
	int				x;

	x = 0;
	while (x < map->w && yvalid_for_map(in->s[in->p]))
	{
		if (!yadd_are(yc_to_upper(in->s[in->p]), x, y, map))
		{
			if (x > 0)
				yfree_ares(map, x, y + 1);
			return (0);
		}
		x++;
		in->p++;
	}
	if (x < map->w || in->s[in->p++] != '\n')
	{
		if (x > 0)
			yfree_ares(map, x, y + 1);
		return (0);
	}
	return (1);
}

static unsigned char	yignore_prefix(t_in *in)
{
	while (in->s[in->p] >= '0' && in->s[in->p] <= '9')
		in->p++;
	if (in->s[in->p++] != ' ')
		return (0);
	return (1);
}

unsigned char			ydecode_map(t_in *in, t_map *map)
{
	int			y;

	y = 0;
	while (y < map->h)
	{
		if (!yignore_prefix(in))
			return (0);
		if (!ydecode_line_map(in, map, y))
		{
			if (y > 0)
				yfree_ares(map, map->w, y);
			return (0);
		}
		y++;
	}
	return (1);
}
