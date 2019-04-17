/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_on_sight.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:04:47 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 14:04:48 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	yseek_target_from_on(t_crd o, char t, t_map *map,
	unsigned char (*yrng_o)(t_map*, t_crd, t_crd*))
{
	t_crd 	n;

	if (yrng_o(map, o, &n) && map->m[n.y][n.x].o == t &&
		ydistance(o, n) < ((double)(map->w + map->h)) / 8)
		return (1);
	return (0);
}

static unsigned char	yseek_target_from(t_crd o, char t, t_map *map)
{
	if (map->m[o.y][o.x].o == t)
		return (1);
	if (yseek_target_from_on(o, t, map, yrng_b_ho))
		return (1);
	if (yseek_target_from_on(o, t, map, yrng_b_lo))
		return (1);
	if (yseek_target_from_on(o, t, map, yrng_h_ho))
		return (1);
	if (yseek_target_from_on(o, t, map, yrng_h_lo))
		return (1);
	if (yseek_target_from_on(o, t, map, yrng_s_ho))
		return (1);
	if (yseek_target_from_on(o, t, map, yrng_s_lo))
		return (1);
	if (yseek_target_from_on(o, t, map, yrng_v_ho))
		return (1);
	if (yseek_target_from_on(o, t, map, yrng_v_lo))
		return (1);
	return (0);
}

unsigned char			yenemy_on_sight(t_gm *gm)
{
	t_pc_nav	nav;

	nav.m_o = (t_crd){gm->map.a.x - gm->pc.map.a.x,
		gm->map.a.y - gm->pc.map.a.y};
	yreset_pc_pos(&gm->pc, &nav.it);
	while (ynext_pc_pos(&gm->me, &gm->pc, &nav.it, &nav.p))
	{
		nav.m_p = (t_crd){nav.m_o.x + nav.p.x, nav.m_o.y + nav.p.y};
		if (!ycoord_equals(nav.p, gm->pc.map.a) &&
			yseek_target_from(nav.m_p, gm->en.o, &gm->map))
			return (1);
	}
	return (0);
}
