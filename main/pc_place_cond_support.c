/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pc_place_cond_support.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:54:57 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 13:54:59 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

unsigned char			yenclosed_piece(t_crd origin, t_pc *pc, t_map *map)
{
	return (yis_coord(origin, map) && origin.x + pc->map.w <= map->w &&
			origin.y + pc->map.h <= map->h);
}

unsigned char			ytest_range(t_crd n, t_crd origin, t_pc *pc, t_map *map)
{
	t_crd		srng;
	t_crd		erng;
	t_crd		en;

	srng = (t_crd){origin.x + n.x, origin.y + n.y};
	if (!yis_coord(srng, map) || map->m[srng.y][srng.x].o != '.')
		return (0);
	yrng_h_hi(map, srng, &erng);
	yrng_h_hi(&(pc->map), n, &en);
	return (en.x + origin.x <= erng.x);
}

unsigned char			ytest_anchor_range(t_crd n, t_crd origin, t_pc *pc,
		t_map *map)
{
	n = (t_crd){n.x + 1, n.y};
	if (yis_coord(n, &(pc->map)) && pc->map.m[n.y][n.x].o != '.')
		return (ytest_range(n, origin, pc, map));
	return (1);
}

unsigned char			ycut_ranges(t_crd n, t_crd origin, t_pc *pc, t_map *map)
{
	t_crd		srng;
	t_crd		erng;

	srng = (t_crd){origin.x + n.x, origin.y + n.y};
	erng = (t_crd){origin.x + pc->map.a.x - 1, origin.y + pc->map.a.y};
	if (!yis_coord(srng, map) || !yis_coord(erng, map) ||
		map->m[srng.y][srng.x].o != '.' ||
		map->m[srng.y][srng.x].h != map->m[erng.y][erng.x].h)
		return (0);
	return (ytest_anchor_range(pc->map.a, origin, pc, map));
}
