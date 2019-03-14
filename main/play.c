/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 18:39:13 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/12 16:36:30 by dabeloos         ###   ########.fr       */
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

static unsigned char	ynext_map_pos(t_ply *ply, t_map *map)
{
	t_crd		li;
	t_crd		hi;

	if (yis_coord(ply->hp, map))
	{
		map->a = ply->hp;
		ynext_hp(ply->o, map, &(ply->hp));
		return (1);
	}
	else
	{
		while (yis_coord(ply->vp, map))
		{
			map->a = ply->vp;
			ynext_vp(ply->o, map, &(ply->vp));
			yrng_h_li(map, map->a, &li);
			yrng_h_hi(map, map->a, &hi);
			if (!ycoord_equals(map->a, li) && !ycoord_equals(map->a, hi))
				return (1);
		}
		return (0);
	}
}

static unsigned char	ynext_pc_pos(t_ply *ply, t_pc *pc)
{
	t_crd		li;
	t_crd		hi;

	if (yis_coord(pc->hp, &(pc->map)))
	{
		pc->map.a = pc->hp;
		ynext_hp(ply->o, &(pc->map), &(pc->hp));
		return (1);
	}
	else
	{
		while (yis_coord(pc->vp, &(pc->map)))
		{
			pc->map.a = pc->vp;
			ynext_vp(ply->o, &(pc->map), &(pc->vp));
			yrng_h_li(&(pc->map), pc->map.a, &li);
			yrng_h_hi(&(pc->map), pc->map.a, &hi);
			if (!ycoord_equals(pc->map.a, li) && !ycoord_equals(pc->map.a, hi))
				return (1);
		}
		return (0);
	}
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
	return (en.x + origin.x <= erng.x);
}

static unsigned char	ytest_anchor_range(t_crd n, t_crd origin, t_pc *pc,
		t_map *map)
{
	n = (t_crd){n.x + 1, n.y};
	if (yis_coord(n, &(pc->map)) && pc->map.m[n.y][n.x].o != '.')
		return (ytest_range(n, origin, pc, map));
	return (1);
}

static unsigned char	ycut_ranges(t_crd n, t_crd origin, t_pc *pc, t_map *map)
{
	t_crd		srng;
	t_crd		erng;

	srng = (t_crd){origin.x + n.x, origin.y + n.y};
	erng = (t_crd){origin.x + pc->map.a.x - 1, origin.y + pc->map.a.y};
	if (!yis_coord(srng, map) || !yis_coord(erng, map) ||
		map->m[srng.y][srng.x].o != '.' ||
		map->m[srng.y][srng.x].h != map->m[erng.y][erng.x].h)
		return (0);
	return (ytest_anchor_range(pc->map.a, origin, pc, map));
}

static unsigned char	ycan_put_piece(t_pc *pc, t_map *map)
{
	t_crd		n;
	t_crd		origin;

	origin = (t_crd){map->a.x - pc->map.a.x, map->a.y - pc->map.a.y};
	if (!yenclosed_piece(origin, pc, map))
	{
		//ft_printf("fail enclosed\n");
		return (0);
	}
	n = (t_crd){0, 0};
	while (n.y < pc->map.h)
	{
		while (pc->map.m[n.y][n.x].o != '.' || yrng_h_ho(&(pc->map), n, &n))
		{
			if (ycoord_equals(n, pc->map.a))
			{
				if (!ytest_anchor_range(n, origin, pc, map))
				{
					//ft_printf("fail anchor\n");
					return (0);
				}
			}
			else if (n.y == pc->map.a.y &&
				pc->map.m[n.y][n.x].h->s.x < pc->map.a.x &&
				pc->map.m[n.y][n.x].h->s.x +
				pc->map.m[n.y][n.x].h->d > pc->map.a.x)
			{
				if (!ycut_ranges(n, origin, pc, map))
				{
					//ft_printf("fail cut\n");
					return (0);
				}
			}
			else if (!ytest_range(n, origin, pc, map))
			{
				//ft_printf("fail std\n");
				return (0);
			}
			if (!yrng_h_ho(&(pc->map), n, &n))
				break ;
		}
		n = (t_crd){0, n.y + 1};
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

static void				yidentify_quarter(t_crd o, t_crd d, t_crd *s, t_crd *e)
{
	if (o.x < d.x)
	{
		*s = (t_crd){o.x, d.y};
		*e = (t_crd){d.x, o.y};
	}
	else
	{
		*s = (t_crd){d.x, o.y};
		*e = (t_crd){o.x, d.y};
	}
}

static unsigned char	yfit_backslash(t_gm *gm, t_crd *s, t_crd *e)
{
	if (s->x < 0)
		*s = (t_crd){0, s->y - s->x};
	if (s->y < 0)
		*s = (t_crd){s->x - s->y, 0};
	if (!yis_coord(*s, &gm->map))
		return (0);
	if (e->x >= gm->map.w)
		*e = (t_crd){gm->map.w - 1, e->y - (e->x - gm->map.w + 1)};
	if (e->y >= gm->map.h)
		*e = (t_crd){e->x - (e->y - gm->map.h + 1), gm->map.h - 1};
	if (!yis_coord(*e, &gm->map))
		return (0);
	return (1);
}

static unsigned char	yfit_slash(t_gm *gm, t_crd *s, t_crd *e)
{
	if (s->x < 0)
		*s = (t_crd){0, s->y - s->x};
	if (s->y >= gm->map.h)
		*s = (t_crd){s->x - (s->y - gm->map.h + 1), gm->map.h - 1};
	if (!yis_coord(*s, &gm->map))
		return(0);
	if (e->x >= gm->map.w)
		*e = (t_crd){gm->map.w - 1, e->y - (e->x - gm->map.w + 1)};
	if (e->y < 0)
		*e = (t_crd){e->x - e->y, 0};
	if (!yis_coord(*e, &gm->map))
		return (0);
	return (1);
}

static t_crd			ysonar_backslash(t_gm *gm, char t, t_crd s, t_crd e)
{
	t_crd				d;

	if (!yfit_backslash(gm, &s, &e))
		return ((t_crd){-1, -1});
	d = s;
	if (gm->map.m[d.y][d.x].o == t)
		return (d);
	while (yrng_b_ho(&gm->map, d, &d))
	{
		if (d.x > e.x)
			return ((t_crd){-1, -1});
		if (gm->map.m[d.y][d.x].o == t)
			return (d);
	}
	return ((t_crd){-1, -1});
}

static t_crd			ysonar_slash(t_gm *gm, char t, t_crd s, t_crd e)
{
	t_crd				d;

	if (!yfit_slash(gm, &s, &e))
		return ((t_crd){-1, -1});
	d = s;
	if (gm->map.m[d.y][d.x].o == t)
		return (d);
	while (yrng_s_ho(&gm->map, d, &d))
	{
		if (d.x > e.x)
			return ((t_crd){-1, -1});
		if (gm->map.m[d.y][d.x].o == t)
			return (d);
	}
	return ((t_crd){-1, -1});
}

static t_crd			ysonar_quarter(t_gm *gm, t_crd o, char t, t_crd d)
{
	t_crd				s;
	t_crd				e;

	yidentify_quarter(o, d, &s, &e);
	if (s.y > e.y)
		return (ysonar_backslash(gm, t, s, e));
	else
		return (ysonar_slash(gm, t, s, e));
}

static t_crd			ysonar(t_gm *gm, t_crd o, char t, int size)
{
	t_crd				out;

	if (yis_coord((out = ysonar_quarter(gm, o, t, (t_crd){o.x + size, o.y +
		size})), &gm->map))
		return (out);
	if (yis_coord((out = ysonar_quarter(gm, o, t, (t_crd){o.x + size, o.y -
		size})), &gm->map))
		return (out);
	if (yis_coord((out = ysonar_quarter(gm, o, t, (t_crd){o.x - size, o.y -
		size})), &gm->map))
		return (out);
	if (yis_coord((out = ysonar_quarter(gm, o, t, (t_crd){o.x - size, o.y +
		size})), &gm->map))
		return (out);
	return ((t_crd){-1, -1});
}

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
		size++;
	return ((t_crd){-1, -1});
}

unsigned char			yplay(t_gm *gm)
{
	if (!yfind_start_positions(gm))
		return (0);
	while (ynext_map_pos(&(gm->me), &(gm->map)))
	{
		//ft_printf("\nmap pos : %d %d\n", gm->map.a.x, gm->map.a.y);
		fprintf(2, "\n%d %d\n", yfind_nearest(gm, gm->map.a, gm->en.o));
		yreset_pc_pos(&(gm->pc));
		while (ynext_pc_pos(&(gm->me), &(gm->pc)))
		{
			//ft_printf("pc pos : %d %d\n", gm->pc.map.a.x, gm->pc.map.a.y);
			if (ycan_put_piece(&(gm->pc), &(gm->map)))
			{
				yput_piece(&(gm->map), &(gm->pc));
				return (1);
			}
		}
	}
	return (0);
}
