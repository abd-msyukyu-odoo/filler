/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rng_h.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:42:11 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 13:42:12 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

unsigned char			yrng_h_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x + map->m[in.y][in.x].h->d,
		map->m[in.y][in.x].h->s.y};
	return (out->x < map->w);
}

unsigned char			yrng_h_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x - 1,
		map->m[in.y][in.x].h->s.y};
	return (out->x > 0);
}

void					yrng_h_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x + map->m[in.y][in.x].h->d - 1,
		map->m[in.y][in.x].h->s.y};
}

void					yrng_h_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x,
		map->m[in.y][in.x].h->s.y};
}
