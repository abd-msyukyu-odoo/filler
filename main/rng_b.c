/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rng_b.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:43:35 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 13:43:36 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

unsigned char			yrng_b_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b->s.x + map->m[in.y][in.x].b->d,
		map->m[in.y][in.x].b->s.y + map->m[in.y][in.x].b->d};
	return (out->x < map->w && out->y < map->h);
}

unsigned char			yrng_b_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b->s.x - 1,
		map->m[in.y][in.x].b->s.y - 1};
	return (out->x > 0 && out->y > 0);
}
