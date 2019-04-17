/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   position_iterator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:51:51 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 13:51:52 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static void				ynext_hp(char o, t_map *map, t_crd *hp)
{
	t_crd		verif;

	yrng_h_hi(map, *hp, &verif);
	if (!ycoord_equals(*hp, verif))
	{
		*hp = verif;
		return ;
	}
	while (yrng_h_ho(map, *hp, hp))
		if (map->m[hp->y][hp->x].o == o)
			return ;
	*hp = (t_crd){0, (*hp).y + 1};
	if (yis_coord(*hp, map))
	{
		if (map->m[hp->y][hp->x].o == o)
			return ;
		while (yrng_h_ho(map, *hp, hp))
			if (map->m[hp->y][hp->x].o == o)
				return ;
	}
	*hp = (t_crd){-1, -1};
}

static void				ynext_vp(char o, t_map *map, t_crd *vp)
{
	t_crd		verif;

	yrng_v_hi(map, *vp, &verif);
	if (!ycoord_equals(*vp, verif))
	{
		*vp = verif;
		return ;
	}
	while (yrng_v_ho(map, *vp, vp))
		if (map->m[vp->y][vp->x].o == o)
			return ;
	*vp = (t_crd){(*vp).x + 1, 0};
	if (yis_coord(*vp, map))
	{
		if (map->m[vp->y][vp->x].o == o)
			return ;
		while (yrng_v_ho(map, *vp, vp))
			if (map->m[vp->y][vp->x].o == o)
				return ;
	}
	*vp = (t_crd){-1, -1};
}

unsigned char			ynext_map_pos(t_ply *ply, t_map *map, t_m_it *it,
	t_crd *anchor)
{
	t_crd		li;
	t_crd		hi;

	if (yis_coord(it->hp, map))
	{
		*anchor = it->hp;
		ynext_hp(ply->o, map, &(it->hp));
		return (1);
	}
	else
	{
		while (yis_coord(it->vp, map))
		{
			*anchor = it->vp;
			ynext_vp(ply->o, map, &(it->vp));
			yrng_h_li(map, *anchor, &li);
			yrng_h_hi(map, *anchor, &hi);
			if (!ycoord_equals(*anchor, li) && !ycoord_equals(*anchor, hi))
				return (1);
		}
		return (0);
	}
}

unsigned char			ynext_pc_pos(t_ply *ply, t_pc *pc, t_m_it *it,
	t_crd *anchor)
{
	t_crd		li;
	t_crd		hi;

	if (yis_coord(it->hp, &(pc->map)))
	{
		*anchor = it->hp;
		ynext_hp(ply->o, &(pc->map), &(it->hp));
		return (1);
	}
	else
	{
		while (yis_coord(it->vp, &(pc->map)))
		{
			*anchor = it->vp;
			ynext_vp(ply->o, &(pc->map), &(it->vp));
			yrng_h_li(&(pc->map), *anchor, &li);
			yrng_h_hi(&(pc->map), *anchor, &hi);
			if (!ycoord_equals(*anchor, li) && !ycoord_equals(*anchor, hi))
				return (1);
		}
		return (0);
	}
}

void					yreset_pc_pos(t_pc *pc, t_m_it *it)
{
	it->hp = (t_crd){0, 0};
	if (pc->map.m[0][0].o == '.')
		yrng_h_ho(&(pc->map), it->hp, &(it->hp));
	it->vp = (t_crd){0, 0};
	if (pc->map.m[0][0].o == '.')
		yrng_v_ho(&(pc->map), it->vp, &(it->vp));
}
