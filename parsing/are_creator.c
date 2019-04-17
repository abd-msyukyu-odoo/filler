/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   are_creator.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 12:17:12 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 12:17:13 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	yadd_are_vh(int x, int y, t_map *map)
{
	if (x == 0 || map->m[y][x].o != map->m[y][x - 1].o)
	{
		if (!ymalloc_range(&(map->m[y][x].h), x, y))
			return (0);
	}
	else
		yappend_range(&(map->m[y][x].h), map->m[y][x - 1].h);
	if (y == 0 || map->m[y][x].o != map->m[y - 1][x].o)
	{
		if (!ymalloc_range(&(map->m[y][x].v), x, y))
		{
			if (x == 0)
				free(map->m[y][x].h);
			return (0);
		}
	}
	else
		yappend_range(&(map->m[y][x].v), map->m[y - 1][x].v);
	return (1);
}

static unsigned char	yabort_are_bs(int x, int y, t_map *map)
{
	if (x == 0)
		free(map->m[y][x].h);
	if (y == 0)
		free(map->m[y][x].v);
	if (x == map->w - 1 && y == 0)
		free(map->m[y][x].b);
	return (0);
}

static unsigned char	yadd_are_bs(int x, int y, t_map *map)
{
	if (x == 0 || y == 0 || map->m[y][x].o != map->m[y - 1][x - 1].o)
	{
		if (!ymalloc_range(&(map->m[y][x].b), x, y))
			return (yabort_are_bs(x, y, map));
	}
	else
		yappend_range(&(map->m[y][x].b), map->m[y - 1][x - 1].b);
	if (x == map->w - 1 || y == 0 || map->m[y][x].o != map->m[y - 1][x + 1].o)
	{
		if (!ymalloc_range(&(map->m[y][x].s), x, y))
			return (yabort_are_bs(x, y, map));
	}
	else
	{
		yappend_range(&(map->m[y][x].s), map->m[y - 1][x + 1].s);
		map->m[y][x].s->s = (t_crd){x, y};
	}
	return (1);
}

unsigned char			yadd_are(char o, int x, int y, t_map *map)
{
	map->m[y][x].o = o;
	if (!yadd_are_vh(x, y, map))
		return (0);
	if (!yadd_are_bs(x, y, map))
		return (0);
	return (1);
}
