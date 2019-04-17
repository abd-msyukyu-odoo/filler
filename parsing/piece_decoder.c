/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piece_decoder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 12:23:55 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 12:23:56 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	ydecode_line_pc(t_in *in, t_map *map, int y, char o)
{
	int				x;

	x = 0;
	while (x < map->w && yvalid_for_piece(in->s[in->p]))
	{
		in->s[in->p] = (in->s[in->p] == '*') ? o : in->s[in->p];
		if (!yadd_are(in->s[in->p], x, y, map))
		{
			if (x > 0)
				yfree_ares(map, x, x + 1);
			return (0);
		}
		x++;
		in->p++;
	}
	return (1);
}

unsigned char			ydecode_pc(t_in *in, t_pc *pc, char o)
{
	int			y;

	in->p += (pc->map.w + 1) * pc->mic.y;
	pc->map = (t_map){pc->mac.x + 1 - pc->mic.x, pc->mac.y + 1 - pc->mic.y,
		NULL, (t_crd){-1, -1}};
	if (!ymalloc_map(&(pc->map)))
		return (0);
	y = 0;
	while (y < pc->map.h)
	{
		in->p += pc->mic.x;
		if (!ydecode_line_pc(in, &(pc->map), y, o))
		{
			if (y > 0)
				yfree_ares(&(pc->map), pc->map.w, y);
			return (0);
		}
		while (in->s[in->p++] != '\n')
			;
		++y;
	}
	return (1);
}
