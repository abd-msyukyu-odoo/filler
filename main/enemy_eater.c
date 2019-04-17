/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_eater.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:07:29 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 14:07:30 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	yseek_eater(t_crd o, char t, t_map *map,
	unsigned char (*yrng_o)(t_map*, t_crd, t_crd*), int *dist)
{
	t_crd	c;

	c = o;
	while (yrng_o(map, c, &c))
		if (map->m[c.y][c.x].o == t)
		{
			*dist += ydistance(c, o);
			return (1);
		}
	return (0);
}

static unsigned char	yeaten_partial(t_crd o, char t, t_map *map)
{
	int		eaten;
	int		dist;

	eaten = 0;
	dist = 0;
	if (map->m[o.y][o.x].o == t)
		return (1);
	if (yseek_eater(o, t, map, yrng_b_ho, &dist))
		eaten++;
	if (yseek_eater(o, t, map, yrng_b_lo, &dist))
		eaten++;
	if (yseek_eater(o, t, map, yrng_h_ho, &dist))
		eaten++;
	if (yseek_eater(o, t, map, yrng_h_lo, &dist))
		eaten++;
	if (yseek_eater(o, t, map, yrng_s_ho, &dist))
		eaten++;
	if (yseek_eater(o, t, map, yrng_s_lo, &dist))
		eaten++;
	if (yseek_eater(o, t, map, yrng_v_ho, &dist))
		eaten++;
	if (yseek_eater(o, t, map, yrng_v_lo, &dist))
		eaten++;
	return (eaten >= 5 && (double)dist / eaten < ((double)(map->w + map->h)) / 8);
}

unsigned char			yeaten(t_gm *gm)
{
	t_pc_nav	nav;
	int				n;
	int				eaten;

	nav.m_o = (t_crd){gm->map.a.x - gm->pc.map.a.x,
		gm->map.a.y - gm->pc.map.a.y};
	yreset_pc_pos(&gm->pc, &nav.it);
	n = 0;
	eaten = 0;
	while (ynext_pc_pos(&gm->me, &gm->pc, &nav.it, &nav.p))
	{
		nav.m_p = (t_crd){nav.m_o.x + nav.p.x, nav.m_o.y + nav.p.y};
		if (ycoord_equals(nav.p, gm->pc.map.a))
			continue ;
		n++;
		if (yeaten_partial(nav.m_p, gm->en.o, &gm->map))
			eaten++;
	}
	return (n / 2 <= eaten);
}
