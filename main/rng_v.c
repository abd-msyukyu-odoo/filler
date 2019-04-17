/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rng_v.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:39:56 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 13:39:57 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

unsigned char			yrng_v_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y + map->m[in.y][in.x].v->d};
	return (out->y < map->h);
}

unsigned char			yrng_v_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y - 1};
	return (out->y > 0);
}

void					yrng_v_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y + map->m[in.y][in.x].v->d - 1};
}
