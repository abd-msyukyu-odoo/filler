/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 18:39:13 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/06 13:20:44 by dabeloos         ###   ########.fr       */
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

static unsigned char	yrng_v_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v.s.x,
		map->m[in.y][in.x].v.s.y + map->m[in.y][in.x].v.d};
	return (out->y < map->h);
}

static unsigned char	yrng_v_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v.s.x,
		map->m[in.y][in.x].v.s.y - 1};
	return (out->y > 0);
}

static void				yrng_v_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v.s.x,
		map->m[in.y][in.x].v.s.y + map->m[in.y][in.x].v.d - 1};
}

static void				yrng_v_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v.s.x,
		map->m[in.y][in.x].v.s.y};
}

static unsigned char	yrng_h_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h.s.x + map->m[in.y][in.x].h.d,
		map->m[in.y][in.x].h.s.y};
	return (out->x < map->w);
}

static unsigned char	yrng_h_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h.s.x - 1,
		map->m[in.y][in.x].h.s.y};
	return (out->x > 0);
}

static void				yrng_h_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h.s.x + map->m[in.y][in.x].h.d - 1,
		map->m[in.y][in.x].h.s.y};
}

static void				yrng_h_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h.s.x,
		map->m[in.y][in.x].h.s.y};
}

static unsigned char	yrng_b_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b.s.x + map->m[in.y][in.x].b.d,
		map->m[in.y][in.x].b.s.y + map->m[in.y][in.x].b.d};
	return (out->x < map->w && out->y < map->h);
}

static unsigned char	yrng_b_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b.s.x - 1,
		map->m[in.y][in.x].b.s.y - 1};
	return (out->x > 0 && out->y > 0);
}

static void				yrng_b_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b.s.x + map->m[in.y][in.x].b.d - 1,
		map->m[in.y][in.x].b.s.y + map->m[in.y][in.x].b.d - 1};
}

static void				yrng_b_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b.s.x,
		map->m[in.y][in.x].b.s.y};
}

static unsigned char	yrng_s_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s.s.x + map->m[in.y][in.x].s.d,
		map->m[in.y][in.x].s.s.y - map->m[in.y][in.x].s.d};
	return (out->x < map->w && out->y > 0);
}

static unsigned char	yrng_s_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s.s.x - 1,
		map->m[in.y][in.x].s.s.y + 1};
	return (out->x > 0 && out->y < map->h);
}

static void				yrng_s_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s.s.x + map->m[in.y][in.x].s.d - 1,
		map->m[in.y][in.x].s.s.y - map->m[in.y][in.x].s.d + 1};
}

static void				yrng_s_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s.s.x,
		map->m[in.y][in.x].s.s.y};
}

static void				ynext_map_hp(t_ply *ply, t_map *map)
{
	t_crd		head;

	head = (t_crd){map->m[ply->hp.y][ply->hp.x].h->
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
