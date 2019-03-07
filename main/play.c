/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 18:39:13 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/07 15:30:28 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	yis_coord(t_crd crd, t_map *map)
{
	if (!map)
		return (crd.x > -1 && crd.y > -1);
	return (crd.x > -1 && crd.y > -1 && crd.x < map->w &&
			crd.y < map->h);
}

static unsigned char	ycoord_equals(t_crd c1, t_crd c2)
{
	return (c1.x == c2.x && c1.y == c2.y);
}

static unsigned char	yrng_v_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y + map->m[in.y][in.x].v->d};
	return (out->y < map->h);
}

static unsigned char	yrng_v_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y - 1};
	return (out->y > 0);
}

static void				yrng_v_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y + map->m[in.y][in.x].v->d - 1};
}

static void				yrng_v_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y};
}

static unsigned char	yrng_h_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x + map->m[in.y][in.x].h->d,
		map->m[in.y][in.x].h->s.y};
	ft_printf("%d %d\n", out->x, out->y); //ici
	return (out->x < map->w);
}

static unsigned char	yrng_h_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x - 1,
		map->m[in.y][in.x].h->s.y};
	return (out->x > 0);
}

static void				yrng_h_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x + map->m[in.y][in.x].h->d - 1,
		map->m[in.y][in.x].h->s.y};
}

static void				yrng_h_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x,
		map->m[in.y][in.x].h->s.y};
}

static unsigned char	yrng_b_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b->s.x + map->m[in.y][in.x].b->d,
		map->m[in.y][in.x].b->s.y + map->m[in.y][in.x].b->d};
	return (out->x < map->w && out->y < map->h);
}

static unsigned char	yrng_b_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b->s.x - 1,
		map->m[in.y][in.x].b->s.y - 1};
	return (out->x > 0 && out->y > 0);
}

static void				yrng_b_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b->s.x + map->m[in.y][in.x].b->d - 1,
		map->m[in.y][in.x].b->s.y + map->m[in.y][in.x].b->d - 1};
}

static void				yrng_b_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b->s.x,
		map->m[in.y][in.x].b->s.y};
}

static unsigned char	yrng_s_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s->s.x + map->m[in.y][in.x].s->d,
		map->m[in.y][in.x].s->s.y - map->m[in.y][in.x].s->d};
	return (out->x < map->w && out->y > 0);
}

static unsigned char	yrng_s_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s->s.x - 1,
		map->m[in.y][in.x].s->s.y + 1};
	return (out->x > 0 && out->y < map->h);
}

static void				yrng_s_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s->s.x + map->m[in.y][in.x].s->d - 1,
		map->m[in.y][in.x].s->s.y - map->m[in.y][in.x].s->d + 1};
}

static void				yrng_s_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s->s.x,
		map->m[in.y][in.x].s->s.y};
}

static unsigned char	yidentify_pos_vp(t_gm *gm, t_crd in)
{
	if (!yis_coord(gm->en.vp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->en.o)
		gm->en.vp = in;
	else if (!yis_coord(gm->me.vp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->me.o)
		gm->me.vp = in;
	if (yis_coord(gm->en.vp, &(gm->map)) && yis_coord(gm->me.vp, &(gm->map)))
		return (1);
	return (0);
}

static unsigned char	yfind_start_vp(t_gm *gm)
{
	t_crd		in;

	in = (t_crd){0, 0};
	gm->me.vp = (t_crd){-1, -1};
	gm->en.vp = (t_crd){-1, -1};
	while (in.x < gm->map.w)
	{
		if (yidentify_pos_vp(gm, in))
			return (1);
		while (yrng_v_ho(&(gm->map), in, &(in)))
			if (yidentify_pos_vp(gm, in))
				return (1);
		in = (t_crd){in.x + 1, 0};
	}
	gm->en.o = '\0';
	if (!yis_coord(gm->me.vp, &(gm->map)))
		return (0);
	return (1);
}

static unsigned char	yidentify_pos_hp(t_gm *gm, t_crd in)
{
	if (!yis_coord(gm->en.hp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->en.o)
		gm->en.hp = in;
	else if (!yis_coord(gm->me.hp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->me.o)
		gm->me.hp = in;
	if (yis_coord(gm->en.hp, &(gm->map)) && yis_coord(gm->me.hp, &(gm->map)))
		return (1);
	return (0);
}

static unsigned char	yfind_start_hp(t_gm *gm)
{
	t_crd		in;

	in = (t_crd){0, 0};
	gm->me.hp = (t_crd){-1, -1};
	gm->en.hp = (t_crd){-1, -1};
	while (in.y < gm->map.h)
	{
		if (yidentify_pos_hp(gm, in))
			return (1);
		while (yrng_h_ho(&(gm->map), in, &(in)))
			if (yidentify_pos_hp(gm, in))
				return (1);
		in = (t_crd){0, in.y + 1};
	}
	gm->en.o = '\0';
	if (!yis_coord(gm->me.hp, &(gm->map)))
		return (0);
	return (1);
}

static unsigned char	yfind_start_positions(t_gm *gm)
{
	return (yfind_start_hp(gm) && yfind_start_vp(gm));
}

static void				yreset_pc_pos(t_pc *pc)
{
	pc->hp = (t_crd){0, 0};
	if (pc->map.m[0][0].o == '.')
		yrng_h_ho(&(pc->map), pc->hp, &(pc->hp));
	pc->vp = (t_crd){0, 0};
	if (pc->map.m[0][0].o == '.')
		yrng_v_ho(&(pc->map), pc->vp, &(pc->vp));
}

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
	*vp = (t_crd){-1, -1};
}

static unsigned char	ynext_map_pos(t_ply *ply, t_map *map)
{
	if (yis_coord(ply->hp, map))
	{
		map->a = ply->hp;
		ynext_hp(ply->o, map, &(ply->hp));
		return (1);
	}
	else if (yis_coord(ply->vp, map))
	{
		map->a = ply->vp;
		ynext_vp(ply->o, map, &(ply->vp));
		return (1);
	}
	else
		return (0);
}

static unsigned char	ynext_pc_pos(t_ply *ply, t_pc *pc)
{
	if (yis_coord(pc->hp, &(pc->map)))
	{
		pc->map.a = pc->hp;
		ynext_hp(ply->o, &(pc->map), &(pc->hp));
		return (1);
	}
	else if (yis_coord(pc->vp, &(pc->map)))
	{
		pc->map.a = pc->vp;
		ynext_vp(ply->o, &(pc->map), &(pc->hp));
		return (1);
	}
	else
		return (0);
}

static unsigned char	yenclosed_piece(t_crd origin, t_pc *pc, t_map *map)
{
	return (yis_coord(origin, map) && origin.x + pc->map.w <= map->w &&
			origin.y + pc->map.h <= map->h);
}

static unsigned char	ytest_range(t_crd n, t_crd origin, t_pc *pc, t_map *map)
{
	t_crd		srng;
	t_crd		erng;
	t_crd		en;

	srng = (t_crd){origin.x + n.x, origin.y + n.y};
	if (!yis_coord(srng, map) || map->m[srng.y][srng.x].o != '.')
		return (0);
	yrng_h_hi(map, srng, &erng);
	yrng_h_hi(&(pc->map), n, &en);
	return (en.y <= erng.y);
}

static unsigned char	ycut_ranges(t_crd n, t_crd origin, t_pc *pc, t_map *map)
{
	t_crd		n2;

	n2 = (t_crd){pc->map.a.x + 1, pc->map.a.y};
	return (ytest_range(n, origin, pc, map) &&
			ytest_range(n2, origin, pc, map));
}

static unsigned char	ycan_put_piece(t_pc *pc, t_map *map)
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
			if (n.y == pc->map.a.y &&
				pc->map.m[n.y][n.x].h->s.x <= pc->map.a.x &&
				pc->map.m[n.y][n.x].h->s.x +
				pc->map.m[n.y][n.x].h->d > pc->map.a.x &&
				!ycut_ranges(n, origin, pc, map))
				return (0);
			else if (!ytest_range(n, origin, pc, map))
				return (0);
			if (!yrng_h_ho(&(pc->map), n, &n))
				break ;
		}
		n = (t_crd){n.x, n.y + 1};
	}
	return (1);
}

static void				yput_piece(t_map *map, t_pc *pc)
{
	t_crd		origin;

	origin = (t_crd){map->a.x - pc->map.a.x - pc->mic.x,
		map->a.y - pc->map.a.y - pc->mic.y};
	ft_printf("%d %d\n", origin.y, origin.x);
}

void					yplay(t_gm *gm)
{
	if (!yfind_start_positions(gm))
		return ;
	while (ynext_map_pos(&(gm->me), &(gm->map)))
	{
		yreset_pc_pos(&(gm->pc));
		while (ynext_pc_pos(&(gm->me), &(gm->pc)))
		{
			if (ycan_put_piece(&(gm->pc), &(gm->map)))
			{
				yput_piece(&(gm->map), &(gm->pc));
				return ;
			}
		}
	}
}
