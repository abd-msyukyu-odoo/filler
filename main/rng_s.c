/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rng_s.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:44:55 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 13:44:56 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

unsigned char			yrng_s_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s->s.x + map->m[in.y][in.x].s->d,
		map->m[in.y][in.x].s->s.y - map->m[in.y][in.x].s->d};
	return (out->x < map->w && out->y > 0);
}

unsigned char			yrng_s_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s->s.x - 1,
		map->m[in.y][in.x].s->s.y + 1};
	return (out->x > 0 && out->y < map->h);
}
