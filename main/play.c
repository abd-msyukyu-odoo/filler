/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 18:39:13 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/06 12:17:43 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	yis_coord(t_crd *crd)
{
	return (crd->x ! -1 && crd->y != -1);
}

static unsigned char	yfind_start_vp(t_gm *gm)
{
	int			y;
	int			x;

	gm->me.s = (t_crd){-1, -1};
	gm->en.s = (t_crd){-1, -1};
	x = 0;
	while (x < gm->map.w)
	{
		y = 0;
		while (y < gm->map.h)
		{
			if (!yis_coord(gm->en.vp) && gm->map.m[y][x].o == gm->en.o)
				gm->en.vp = (t_crd){x, y};
			else if (!yis_coord(gm->me.vp) && gm->map.m[y][x].o == gm->me.o)
				gm->me.vp = (t_crd){x, y};
			if (yis_coord(gm->en.vp) && yis_coord(gm->me.vp))
				return (1);
			y++;
		}
		x++;
	}
	gm->en.o = '\0';
	if (!yis_coord(gm->me.vp))
		return (0);
	return (1);
}

static unsigned char	yfind_start_hp(t_gm *gm)
{
	int			y;
	int			x;

	gm->me.s = (t_crd){-1, -1};
	gm->en.s = (t_crd){-1, -1};
	y = 0;
	while (y < gm->map.h)
	{
		x = 0;
		while (x < gm->map.w)
		{
			if (!yis_coord(gm->en.hp) && gm->map.m[y][x].o == gm->en.o)
				gm->en.hp = (t_crd){x, y};
			else if (!yis_coord(gm->me.hp) && gm->map.m[y][x].o == gm->me.o)
				gm->me.hp = (t_crd){x, y};
			if (yis_coord(gm->en.hp) && yis_coord(gm->me.hp))
				return (1);
			x++;
		}
		y++;
	}
	gm->en.o = '\0';
	if (!yis_coord(gm->me.hp))
		return (0);
	return (1);
}

static unsigned char	yfind_start_positions(t_gm *gm)
{
	return (yfind_start_hp(gm) && yfind_start_vp(gm));
}

static void				yreset_pc_pos(t_pc *pc)
{
	int			p;

	p = 0;
	while (p < pc->map.w)
	{
		if (pc->map.m[0][p] != '.')
		{
			pc->hp = {p, 0};
			return ;
		}
		p++;
	}
	p = 0;
	while (p < pc->map.h)
	{
		if (pc->map.m[p][0] != '.')
		{
			pc->vp = {0, p};
			return ;
		}
		p++;
	}
}

static void				ynext_map_hp(t_ply *ply, t_map *map)
{
	
}

static unsigned char	ynext_map_pos(t_ply *ply, t_map *map, t_crd *am)
{
	if (yis_coord(ply->hp))
	{
		*am = ply->hp;
		ynext_map_hp(t_ply *ply, t_map *map);
		return (1);
	}
	else if (yis_coord(ply->vp))
	{
		*am = ply->vp;
		ynext_map_vp(t_ply *ply, t_map *map);
		return (1);
	}
	else
		return (0);
}

void					yplay(t_gm *gm)
{
	//chercher une case de contour du joueur sur la map
	//chercher une case de contour de la piece (attention debordement)
	//tester si la piece peut se mettre
	//a partir d'une piece d'un joueur, on peut facilement trouver toutes les
	//autres
	t_crd			am;
	t_crd			ap;

	if (!yfind_start_positions(gm))
		return ;
	while (ynext_map_pos(&(gm->me), &(gm->map), &am))
	{
		yreset_pc_pos(&(gm->pc));
		while (ynext_pc_pos(&(gm->pc))
		{
			if (ycan_put_piece(&(gm->me), &(gm->pc)))
				yput_piece(&(gm->pc.s));
		}
	}
}
