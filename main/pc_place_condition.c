/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pc_place_condition.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:58:23 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 13:58:24 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

unsigned char			ycan_put_piece(t_pc *pc, t_map *map)
{
	t_crd		n;
	t_crd		origin;

	origin = (t_crd){map->a.x - pc->map.a.x, map->a.y - pc->map.a.y};
	if (!yenclosed_piece(origin, pc, map))
		return (0);
	n = (t_crd){0, 0};
	while (n.y < pc->map.h)
	{
		while (pc->map.m[n.y][n.x].o != '.' || yrng_h_ho(&(pc->map), n, &n))
		{
			if (ycoord_equals(n, pc->map.a))
			{
				if (!ytest_anchor_range(n, origin, pc, map))
					return (0);
			}
			else if (n.y == pc->map.a.y &&
				pc->map.m[n.y][n.x].h->s.x < pc->map.a.x &&
				pc->map.m[n.y][n.x].h->s.x +
				pc->map.m[n.y][n.x].h->d > pc->map.a.x)
			{
				if (!ycut_ranges(n, origin, pc, map))
					return (0);
			}
			else if (!ytest_range(n, origin, pc, map))
				return (0);
			if (!yrng_h_ho(&(pc->map), n, &n))
				break ;
		}
		n = (t_crd){0, n.y + 1};
	}
	return (1);
}
