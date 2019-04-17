/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 12:12:25 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 12:12:26 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

void					ydereference_rng(t_map *map, int x, int y, t_rng *rng)
{
	if (!rng)
		return ;
	while (y-- > 0)
	{
		while (x-- > 0)
		{
			if (map->m[y][x].h == rng)
				map->m[y][x].h = NULL;
			if (map->m[y][x].v == rng)
				map->m[y][x].v = NULL;
			if (map->m[y][x].b == rng)
				map->m[y][x].b = NULL;
			if (map->m[y][x].s == rng)
				map->m[y][x].s = NULL;
		}
		x = map->w;
	}
	free(rng);
}

void					yfree_ares(t_map *map, int x, int y)
{
	while (y-- > 0)
	{
		while (x-- > 0)
		{
			ydereference_rng(map, x, y + 1, map->m[y][x].v);
			ydereference_rng(map, x, y + 1, map->m[y][x].h);
			ydereference_rng(map, x, y + 1, map->m[y][x].b);
			ydereference_rng(map, x, y + 1, map->m[y][x].s);
		}
		x = map->w;
	}
}

void					yfree_map(t_map *map)
{
	int			y;

	y = 0;
	while (y < map->h)
		free(map->m[y++]);
	free(map->m);
}

void					yfree_turn(t_map *map, t_pc *pc)
{
	yfree_ares(&(pc->map), pc->map.w, pc->map.h);
	yfree_map(&(pc->map));
	yfree_ares(map, map->w, map->h);
	yfree_map(map);
}

void					yreset_in(t_in *in)
{
	free(in->s);
	in->p = 0;
}
