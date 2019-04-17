/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:08:33 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 14:08:34 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static void				yput_piece(t_map *map, t_pc *pc)
{
	t_crd		origin;

	origin = (t_crd){map->a.x - pc->map.a.x - pc->mic.x,
		map->a.y - pc->map.a.y - pc->mic.y};
	ft_printf("%d %d\n", origin.y, origin.x);
}

unsigned char			yplay(t_gm *gm)
{
	t_crd			best;
	t_crd			best_map;
	int				score;
	int				cur_score;
	unsigned char	on_sight;
	unsigned char	cur_on_sight;
	unsigned char	not_eaten;
	unsigned char	cur_not_eaten;

	best = (t_crd){-1, -1};
	score = -1;
	on_sight = 0;
	not_eaten = 0;
	if (!yfind_start_positions(gm))
		return (0);
	while (ynext_map_pos(&(gm->me), &(gm->map), &gm->me.it, &gm->map.a))
	{
		yreset_pc_pos(&(gm->pc), &gm->pc.it);
		while (ynext_pc_pos(&(gm->me), &(gm->pc), &gm->pc.it, &gm->pc.map.a))
		{
			if (ycan_put_piece(&(gm->pc), &(gm->map)))
			{
				cur_score = yscore_closest(gm);
				cur_on_sight = yenemy_on_sight(gm);
				if (!on_sight)
				{
					if (cur_on_sight)
						on_sight = 1;
					if (on_sight || score == -1 || cur_score < score)
					{
						score = cur_score;
						best = gm->pc.map.a;
						best_map = gm->map.a;
					}
				}
				else if (cur_on_sight)
				{
					cur_not_eaten = !yeaten(gm);
					if (!not_eaten)
					{
						if (cur_not_eaten)
							not_eaten = 1;
						if (not_eaten || score == -1 || cur_score < score)
						{
							score = cur_score;
							best = gm->pc.map.a;
							best_map = gm->map.a;
						}
					}
					else if (cur_not_eaten)
					{
						if (score == -1 || cur_score < score)
						{
							score = cur_score;
							best = gm->pc.map.a;
							best_map = gm->map.a;
						}
					}
				}
			}
		}
	}
	if (yis_coord(best, &gm->map))
	{
		gm->pc.map.a = best;
		gm->map.a = best_map;
		yput_piece(&gm->map, &gm->pc);
		return (1);
	}
	return (0);
}
