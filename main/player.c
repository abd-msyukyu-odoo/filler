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

static void				yupdate_best(t_gm *gm, t_yplay *d)
{
	d->score = d->cur_score;
	d->on_pc = gm->pc.map.a;
	d->on_map = gm->map.a;
}

static void				yscore_play(t_gm *gm, t_yplay *d)
{
	d->cur_score = yscore_closest(gm);
	d->cur_on_sight = yenemy_on_sight(gm);
	if (!d->on_sight)
	{
		if (d->cur_on_sight)
			d->on_sight = 1;
		if (d->on_sight || d->score == -1 || d->cur_score < d->score)
			yupdate_best(gm, d);
	}
	else if (d->cur_on_sight)
	{
		d->cur_not_eaten = !yeaten(gm);
		if (!d->not_eaten)
		{
			if (d->cur_not_eaten)
				d->not_eaten = 1;
			if (d->not_eaten || d->score == -1 || d->cur_score < d->score)
				yupdate_best(gm, d);
		}
		else if (d->cur_not_eaten)
		{
			if (d->score == -1 || d->cur_score < d->score)
				yupdate_best(gm, d);
		}
	}
}

unsigned char			yplay(t_gm *gm)
{
	t_yplay		d;

	d.on_pc = (t_crd){-1, -1};
	d.score = -1;
	d.on_sight = 0;
	d.not_eaten = 0;
	if (!yfind_start_positions(gm))
		return (0);
	while (ynext_map_pos(&(gm->me), &(gm->map), &gm->me.it, &gm->map.a))
	{
		yreset_pc_pos(&(gm->pc), &gm->pc.it);
		while (ynext_pc_pos(&(gm->me), &(gm->pc), &gm->pc.it, &gm->pc.map.a))
		{
			if (ycan_put_piece(&(gm->pc), &(gm->map)))
				yscore_play(gm, &d);
		}
	}
	if (yis_coord(d.on_pc, &gm->pc.map))
	{
		gm->pc.map.a = d.on_pc;
		gm->map.a = d.on_map;
		yput_piece(&gm->map, &gm->pc);
		return (1);
	}
	return (0);
}
