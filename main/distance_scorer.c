/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   distance_scorer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:03:07 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 14:03:08 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static t_crd			yfind_nearest(t_gm *gm, t_crd o, char t)
{
	int					size;
	t_crd				out;

    if (!yis_coord(o, &gm->map) || (gm->me.o != t && gm->en.o == '\0'))
		return ((t_crd){-1, -1});
	if (gm->map.m[o.y][o.x].o == t)
		return (o);
	size = 1;
	while (!yis_coord((out = ysonar(gm, o, t, size)), &gm->map) &&
		size < gm->map.h + gm->map.w)
	{
		size++;
	}
	return (out);
}

int						ydistance(t_crd c1, t_crd c2)
{
	int			tmp;
	int			score;

	tmp = c2.y - c1.y;
	score = (tmp < 0) ? -tmp : tmp;
	tmp = c2.x - c1.x;
	score += (tmp < 0) ? -tmp : tmp;
	return (score);
}

int						yscore_closest(t_gm *gm)
{
	t_pc_nav	nav;
	t_crd		nearest;
	int			score;

	score = 0;
	nav.m_o = (t_crd){gm->map.a.x - gm->pc.map.a.x,
		gm->map.a.y - gm->pc.map.a.y};
	yreset_pc_pos(&gm->pc, &nav.it);
	while (ynext_pc_pos(&gm->me, &gm->pc, &nav.it, &nav.p))
	{
		nav.m_p = (t_crd){nav.m_o.x + nav.p.x, nav.m_o.y + nav.p.y};
		nearest = yfind_nearest(gm, nav.m_p, gm->en.o);
		score += ydistance(nav.m_p, nearest);
	}
	return (score);
}
