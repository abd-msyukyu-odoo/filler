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

static t_rng*			yrng_v_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y + map->m[in.y][in.x].v->d};
	if (out->y < map->h)
		return (map->m[(*out).y][(*out).x].v);
	return (NULL);
}

static t_rng*			yrng_v_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y - 1};
	if (out->y > 0)
		return (map->m[(*out).y][(*out).x].v);
	return (NULL);
}

static t_rng*			yrng_v_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y + map->m[in.y][in.x].v->d - 1};
	return (map->m[(*out).y][(*out).x].v);
}

static t_rng*			yrng_v_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].v->s.x,
		map->m[in.y][in.x].v->s.y};
	return (map->m[(*out).y][(*out).x].v);
}

static t_rng*			yrng_v_hn(t_map *map, t_crd in, t_crd *out)
{
	rng_v_hi(map, in, out);
	if (ycoord_equals(in, *out))
		return (NULL);
	*out = (t_crd){in.x, in.y + 1};
	return (map->m[(*out).y][(*out).x].v);
}

static t_rng*			yrng_h_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x + map->m[in.y][in.x].h->d,
		map->m[in.y][in.x].h->s.y};
	if (out->x < map->w)
		return (map->m[(*out).y][(*out).x].h);
	return (NULL);
}

static t_rng*			yrng_h_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x - 1,
		map->m[in.y][in.x].h->s.y};
	if (out->x > 0)
		return (map->m[(*out).y][(*out).x].h);
	return (NULL);
}

static t_rng*			yrng_h_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x + map->m[in.y][in.x].h->d - 1,
		map->m[in.y][in.x].h->s.y};
	return (map->m[(*out).y][(*out).x].h);
}

static t_rng*			yrng_h_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].h->s.x,
		map->m[in.y][in.x].h->s.y};
	return (map->m[(*out).y][(*out).x].h);
}

static t_rng*			yrng_h_hn(t_map *map, t_crd in, t_crd *out)
{
	rng_h_hi(map, in, out);
	if (ycoord_equals(in, *out))
		return (NULL);
	*out = (t_crd){in.x + 1, in.y};
	return (map->m[(*out).y][(*out).x].h);
}

static t_rng*			yrng_b_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b->s.x + map->m[in.y][in.x].b->d,
		map->m[in.y][in.x].b->s.y + map->m[in.y][in.x].b->d};
	if (out->x < map->w && out->y < map->h)
		return (map->m[(*out).y][(*out).x].b);
	return (NULL);
}

static t_rng*			yrng_b_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b->s.x - 1,
		map->m[in.y][in.x].b->s.y - 1};
	if (out->x > 0 && out->y > 0)
		return (map->m[(*out).y][(*out).x].b);
	return (NULL);
}

static t_rng*			yrng_b_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b->s.x + map->m[in.y][in.x].b->d - 1,
		map->m[in.y][in.x].b->s.y + map->m[in.y][in.x].b->d - 1};
	return (map->m[(*out).y][(*out).x].b);
}

static t_rng*			yrng_b_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].b->s.x,
		map->m[in.y][in.x].b->s.y};
	return (map->m[(*out).y][(*out).x].b);
}

static t_rng*			yrng_b_hn(t_map *map, t_crd in, t_crd *out)
{
	rng_b_hi(map, in, out);
	if (ycoord_equals(in, *out))
		return (NULL);
	*out = (t_crd){in.x + 1, in.y + 1};
	return (map->m[(*out).y][(*out).x].b);
}

static t_rng*			yrng_s_ho(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s->s.x + map->m[in.y][in.x].s->d,
		map->m[in.y][in.x].s->s.y - map->m[in.y][in.x].s->d};
	if (out->x < map->w && out->y > 0)
		return (map->m[(*out).y][(*out).x].s);
	return (NULL);
}

static t_rng*			yrng_s_lo(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s->s.x - 1,
		map->m[in.y][in.x].s->s.y + 1};
	if (out->x > 0 && out->y < map->h)
		return (map->m[(*out).y][(*out).x].s);
	return (NULL);
}

static t_rng*			yrng_s_hi(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s->s.x + map->m[in.y][in.x].s->d - 1,
		map->m[in.y][in.x].s->s.y - map->m[in.y][in.x].s->d + 1};
	return (map->m[(*out).y][(*out).x].s);
}

static t_rng*			yrng_s_li(t_map *map, t_crd in, t_crd *out)
{
	*out = (t_crd){map->m[in.y][in.x].s->s.x,
		map->m[in.y][in.x].s->s.y};
	return (map->m[(*out).y][(*out).x].s);
}

static t_rng*			yrng_s_hn(t_map *map, t_crd in, t_crd *out)
{
	rng_s_hi(map, in, out);
	if (ycoord_equals(in, *out))
		return (NULL);
	*out = (t_crd){in.x + 1, in.y - 1};
	return (map->m[(*out).y][(*out).x].s);
}

static unsigned char	yidentify_pos_vp(t_gm *gm, t_crd in, t_crd *mevp,
	t_crd *envp)
{
	if (!yis_coord(*envp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->en.o)
		*envp = in;
	else if (!yis_coord(*mevp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->me.o)
		*mevp = in;
	if (yis_coord(*envp, &(gm->map)) && yis_coord(*mevp, &(gm->map)))
		return (1);
	return (0);
}

static unsigned char	yfind_start_vp(t_gm *gm, t_crd *mevp, t_crd *envp)
{
	t_crd		in;

	in = (t_crd){0, 0};
	*mevp = (t_crd){-1, -1};
	*envp = (t_crd){-1, -1};
	while (in.x < gm->map.w)
	{
		if (yidentify_pos_vp(gm, in, mevp, envp))
			return (1);
		while (yrng_v_ho(&(gm->map), in, &(in)))
			if (yidentify_pos_vp(gm, in, mevp, envp))
				return (1);
		in = (t_crd){in.x + 1, 0};
	}
	gm->en.o = '\0';
	if (!yis_coord(*mevp, &(gm->map)))
		return (0);
	return (1);
}

static unsigned char	yidentify_pos_hp(t_gm *gm, t_crd in, t_crd *mehp,
	t_crd *enhp)
{
	if (!yis_coord(*enhp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->en.o)
		*enhp = in;
	else if (!yis_coord(*mehp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->me.o)
		*mehp = in;
	if (yis_coord(*enhp, &(gm->map)) && yis_coord(*mehp, &(gm->map)))
		return (1);
	return (0);
}

static unsigned char	yfind_start_hp(t_gm *gm, t_crd *mehp, t_crd *enhp)
{
	t_crd		in;

	in = (t_crd){0, 0};
	*mehp = (t_crd){-1, -1};
	*enhp = (t_crd){-1, -1};
	while (in.y < gm->map.h)
	{
		if (yidentify_pos_hp(gm, in, mehp, enhp))
			return (1);
		while (yrng_h_ho(&(gm->map), in, &(in)))
			if (yidentify_pos_hp(gm, in, mehp, enhp))
				return (1);
		in = (t_crd){0, in.y + 1};
	}
	gm->en.o = '\0';
	if (!yis_coord(*mehp, &(gm->map)))
		return (0);
	return (1);
}

static unsigned char	yfind_start_positions(t_gm *gm)
{
	return (yfind_start_hp(gm, &gm->me.it.hp, &gm->en.it.hp) &&
		yfind_start_vp(gm, &gm->me.it.vp, &gm->en.it.vp));
}

static void				yreset_pc_pos(t_pc *pc, t_m_it *it)
{
	it->hp = (t_crd){0, 0};
	if (pc->map.m[0][0].o == '.')
		yrng_h_ho(&(pc->map), it->hp, &(it->hp));
	it->vp = (t_crd){0, 0};
	if (pc->map.m[0][0].o == '.')
		yrng_v_ho(&(pc->map), it->vp, &(it->vp));
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

static unsigned char	ynext_map_pos(t_ply *ply, t_map *map, t_m_it *it,
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

static unsigned char	ynext_pc_pos(t_ply *ply, t_pc *pc, t_m_it *it,
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
	if (!yfuse_pc(map, pc))
	{
		yfree_turn(map, pc);
		return ;
	}
	ft_printf("%d %d\n", origin.y, origin.x);
	/*FILE *fd = fopen("output_test.txt", "a");
	fprintf(fd, "%d %d\n\n", origin.y, origin.x);
	fclose(fd);
	*/
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
		*s = (t_crd){0, s->y + s->x};
	if (s->y >= gm->map.h)
		*s = (t_crd){s->x + (s->y - gm->map.h + 1), gm->map.h - 1};
	if (!yis_coord(*s, &gm->map))
		return(0);
	if (e->x >= gm->map.w)
		*e = (t_crd){gm->map.w - 1, e->y + (e->x - gm->map.w + 1)};
	if (e->y < 0)
		*e = (t_crd){e->x + e->y, 0};
	if (!yis_coord(*e, &gm->map))
		return (0);
	return (1);
}

static t_crd			ysonar_backslash(t_gm *gm, char t, t_crd s, t_crd e)
{
	t_crd				d;

	//fprintf(stderr, "backslash it : %d %d to %d %d\n", s.x, s.y, e.x, e.y);
	if (!yfit_backslash(gm, &s, &e))
		return ((t_crd){-1, -1});
	//fprintf(stderr, "backslash fit : %d %d to %d %d\n", s.x, s.y, e.x, e.y);
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

	//fprintf(stderr, "slash it : %d %d to %d %d\n", s.x, s.y, e.x, e.y);
	if (!yfit_slash(gm, &s, &e))
		return ((t_crd){-1, -1});
	//fprintf(stderr, "slash fit : %d %d to %d %d\n", s.x, s.y, e.x, e.y);
	d = s;
	if (gm->map.m[d.y][d.x].o == t)
		return (d);
	while (yrng_s_ho(&gm->map, d, &d))
	{
		//fprintf(stderr, "intermediate hop : %d %d\n", d.x, d.y);
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
	if (s.y < e.y)
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

static int				ydistance(t_crd c1, t_crd c2)
{
	int			tmp;
	int			score;

	tmp = c2.y - c1.y;
	score = (tmp < 0) ? -tmp : tmp;
	tmp = c2.x - c1.x;
	score += (tmp < 0) ? -tmp : tmp;
	return (score);
}

static t_crd			yfind_nearest(t_gm *gm, t_crd o, char t)
{
	int					size;
	int					ref;
	t_crd				out;
	t_crd				cur;
	t_crd				verif;

    if (!yis_coord(o, &gm->map) || (gm->me.o != t && gm->en.o == '\0'))
		return ((t_crd){-1, -1});
	if (gm->map.m[o.y][o.x].o == t)
		return (o);
	size = 1;
	//fprintf(stderr, "\nsize : %d\n", size);
	while (!yis_coord((out = ysonar(gm, o, t, size)), &gm->map) &&
		size < gm->map.h + gm->map.w && size < 10)
	{
		size++;
		//fprintf(stderr, "\nsize : %d\n", size);
	}
	if (yis_coord(out, &gm->map))
		return (out);
	out = gm->en.it.hp;
	ref = ydistance(o, out);
	size = size + (ref - size) / 2;
	verif = (t_crd){-1, -1};
	while (size < ref && size > 10)
	{
		if (!yis_coord((cur = ysonar(gm, o, t, size)), &gm->map) &&
			!yis_coord((verif = ysonar(gm, o, t, size - 1)), &gm->map))
			size = size + (ref - size + 1) / 2;
		else
		{
			if (yis_coord(verif, &gm->map))
				out = verif;
			else
				out = cur;
			ref = size;
			size = 10 + (size - 10) / 2;
		}
	}
	return (out);
}

static int				yscore_closest(t_gm *gm)
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

static unsigned char	yseek_target_from_on(t_crd o, char t, t_map *map,
	t_rng* (*yrng_o)(t_map*, t_crd, t_crd*))
{
	t_crd 	n;

	if (yrng_o(map, o, &n) && map->m[n.y][n.x].o == t &&
		ydistance(o, n) < ((double)(map->w + map->h)) / 8)
		return (1);
	return (0);
}

/*
static unsigned char	yseek_target_from_anchor_on(t_crd o, char t, t_map *map,
	t_dir f)
{
	t_crd	n;

	f.yrng_i(map, o, &n);
	if (ycoord_equals(o, n) && f.yrng_o(map, o, &n))
	{
		if (map->m[n.y][n.x].o == t)
			return (1);
		else if (f.yrng_o(map, n, &n) && map->m[n.y][n.x].o == t)
			return (1);
	}
	return (0);
}
*/

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

/*
static unsigned char	yseek_target_from_anchor(t_crd o, char t, t_map *map)
{
	if (map->m[o.y][o.x].o == t)
		return (1);
	if (yseek_target_from_anchor_on(o, t, map, (t_dir){yrng_b_ho, yrng_b_hi}))
		return (1);
	if (yseek_target_from_anchor_on(o, t, map, (t_dir){yrng_b_lo, yrng_b_li}))
		return (1);
	if (yseek_target_from_anchor_on(o, t, map, (t_dir){yrng_h_ho, yrng_h_hi}))
		return (1);
	if (yseek_target_from_anchor_on(o, t, map, (t_dir){yrng_h_lo, yrng_h_li}))
		return (1);
	if (yseek_target_from_anchor_on(o, t, map, (t_dir){yrng_s_ho, yrng_s_hi}))
		return (1);
	if (yseek_target_from_anchor_on(o, t, map, (t_dir){yrng_s_lo, yrng_s_li}))
		return (1);
	if (yseek_target_from_anchor_on(o, t, map, (t_dir){yrng_v_ho, yrng_v_hi}))
		return (1);
	if (yseek_target_from_anchor_on(o, t, map, (t_dir){yrng_v_lo, yrng_v_li}))
		return (1);
	return (0);
}
*/

static unsigned char	yenemy_on_sight(t_gm *gm)
{
	t_pc_nav	nav;

	nav.m_o = (t_crd){gm->map.a.x - gm->pc.map.a.x,
		gm->map.a.y - gm->pc.map.a.y};
	yreset_pc_pos(&gm->pc, &nav.it);
	while (ynext_pc_pos(&gm->me, &gm->pc, &nav.it, &nav.p))
	{
		nav.m_p = (t_crd){nav.m_o.x + nav.p.x, nav.m_o.y + nav.p.y};
		/*
		if (ycoord_equals(nav.p, gm->pc.map.a))
		{
			if (yseek_target_from_anchor(nav.m_p, gm->en.o, &gm->map))
				return (1);
		}
		*/
		if (!ycoord_equals(nav.p, gm->pc.map.a) &&
			yseek_target_from(nav.m_p, gm->en.o, &gm->map))
			return (1);
	}
	return (0);
}

static unsigned char	yseek_eater(t_crd o, char t, t_map *map,
	t_rng* (*yrng_o)(t_map*, t_crd, t_crd*), int *dist)
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

static unsigned char	yeaten(t_gm *gm)
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

/*
unsigned char			yenemy_frontier(t_gm *gm)
{
	t_crd		o;

	o = (t_crd){0, 0};
	while (o.x < gm->map.w)
	{
		if (!yseek_eater(o, gm->en.o, &gm->map, yrng_v_ho))
			break ;
		o.x++;
	}
	if (o.x == gm->map.w)
		return (1);
	o = (t_crd){0, 0};
	while (o.y < gm->map.h)
	{
		if (!yseek_eater(o, gm->en.o, &gm->map, yrng_h_ho))
			break ;
		o.y++;
	}
	if (o.y == gm->map.h)
		return (1);
	return (0);
}
*/

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
	//unsigned char	enemy_frontier;

	best = (t_crd){-1, -1};
	score = -1;
	on_sight = 0;
	not_eaten = 0;
	//enemy_frontier = yenemy_frontier(gm);
	if (!yfind_start_positions(gm))
		return (0);
	while (ynext_map_pos(&(gm->me), &(gm->map), &gm->me.it, &gm->map.a))
	{
		//ft_printf("\nmap pos : %d %d\n", gm->map.a.x, gm->map.a.y);
		//fprintf(stderr, "\nfrom : %d %d\n", gm->map.a.x, gm->map.a.y);
		//t_crd tst = yfind_nearest(gm, gm->map.a, gm->en.o);
		//fprintf(stderr, "\nto : %d %d\n\n\n", tst.x, tst.y);
		yreset_pc_pos(&(gm->pc), &gm->pc.it);
		while (ynext_pc_pos(&(gm->me), &(gm->pc), &gm->pc.it, &gm->pc.map.a))
		{
			//ft_printf("pc pos : %d %d\n", gm->pc.map.a.x, gm->pc.map.a.y);
			if (ycan_put_piece(&(gm->pc), &(gm->map)))
			//minimiser la distance qui a vue sur l'ennemi ? 
			//reconnaitre la configuration en "bouche ouverte" et eviter de jouer
			//dedans
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
					/*
					if (enemy_frontier)
					{
						if (score == -1 || cur_score < score)
						{
							score = cur_score;
							best = gm->pc.map.a;
							best_map = gm->map.a;
						}
						continue ;
					}
					*/
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
				//fprintf(stderr, "\n%d\n", score);
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

static void				yoverwrite_rng(t_rng *new, t_rng *ref, t_map *map, t_crd crd)
{
	t_are		are;

	are = map->m[crd.y][crd.x];
	if (ref == are.b)
		map->m[crd.y][crd.x].b = new;
	else if (ref == are.s)
		map->m[crd.y][crd.x].s = new;
	else if (ref == are.v)
		map->m[crd.y][crd.x].v = new;
	else if (ref == are.h)
		map->m[crd.y][crd.x].h = new;
}

static unsigned char	ycheck_l_me(t_dot d, t_map *map, char t, t_dir dir, t_dot l, t_dot h)
{
	t_crd		it;
	t_rng		*ref;

	if (yis_coord(h.c, map) && map->m[h.c.y][h.c.x].o == t)
	{
		l.r->d += 1 + h.r->d;
		ref = h.r;
		dir.h.yrng_o(map, d.c, &it);
		while (dir.h.yrng_n(map, it, &it))
			yoverwrite_rng(l.r, ref, map, it);
		free(ref);
		ref = d.r;
		yoverwrite_rng(l.r, d.r, map, d.c);
		free(ref);
	}
	else if (yis_coord(h.c, map) && map->m[h.c.y][h.c.x].o == '.')
	{
		l.r->d += 1;
		h.r->d -= 1;
		dir.h.yrng_o(map, d.c, &it);
		h.r->s = it;
		yoverwrite_rng(l.r, d.r, map, d.c);
	}
	else
	{
		l.r->d += 1;
		ref = d.r;
		yoverwrite_rng(l.r, d.r, map, d.c);
		free(ref);
	}
	return (1);
}

static unsigned char	ycheck_l_void(t_dot d, t_map *map, char t, t_dir dir, t_dot l, t_dot h)
{
	if (yis_coord(h.c, map) && map->m[h.c.y][h.c.x].o == t)
	{

	}
	else if (yis_coord(h.c, map) && map->m[h.c.y][h.c.x].o == '.')
	{

	}
	else
	{
		
	}
}

static unsigned char	ycheck_l_other(t_dot d, t_map *map, char t, t_dir dir, t_dot l, t_dot h)
{
	if (yis_coord(h.c, map) && map->m[h.c.y][h.c.x].o == t)
	{

	}
	else if (yis_coord(h.c, map) && map->m[h.c.y][h.c.x].o == '.')
	{

	}
	else
	{
		
	}
}

static unsigned char	ycheck_dir(t_dot d, t_map *map, char t, t_dir dir)
{
	t_dot		l;
	t_dot		h;

	l.r = dir.l.yrng_i(map, d.c, &l.c);
	if (ycoord_equals(d.c, l.c) && !(l.r = dir.l.yrng_o(map, d.c, &l.c)))
		l.c = (t_crd){-1, -1}; // wall
	//color of l is correct if l is a valid coord
	h.r = dir.h.yrng_i(map, d.c, &h.c);
	if (ycoord_equals(d.c, h.c) && !(h.r = dir.h.yrng_o(map, d.c, &h.c)))
		h.c = (t_crd){-1, -1}; // wall
	//color of h is correct if h is a valid coord
	if (yis_coord(l.c, map) && map->m[l.c.y][l.c.x].o == t)
		return ycheck_l_me(d, map, t, dir, l, h);
	else if (yis_coord(l.c, map) && map->m[l.c.y][l.c.x].o == '.')
		return ycheck_l_void(d, map, t, dir, l, h);
	else
		return ycheck_l_other(d, map, t, dir, l, h);
}

static unsigned char	yfuse_pc_elem(t_crd p, t_map *map, char t)
{
	t_dir		dir;
	t_dot		dot;

	dot = (t_dot){p, NULL};
	if (map->m[p.y][p.x].o == t)
		return (1);
	map->m[p.y][p.x].o = t;
	dir = (t_dir){(t_sen){yrng_h_lo, yrng_h_li, NULL},
		(t_sen){yrng_h_ho, yrng_h_hi, yrng_h_hn}};
	dot.r = map->m[p.y][p.x].h;
	if (!ycheck_dir(dot, map, t, dir))
		return (0);
	dir = (t_dir){(t_sen){yrng_v_lo, yrng_v_li, NULL}, 
		(t_sen){yrng_v_ho, yrng_v_hi, yrng_h_hn}};
	dot.r = map->m[p.y][p.x].v;
	if (!ycheck_dir(dot, map, t, dir))
		return (0);
	dir = (t_dir){(t_sen){yrng_b_lo, yrng_b_li, NULL},
		(t_sen){yrng_b_ho, yrng_b_hi, yrng_h_hn}};
	dot.r = map->m[p.y][p.x].b;
	if (!ycheck_dir(dot, map, t, dir))
		return (0);
	dir = (t_dir){(t_sen){yrng_s_lo, yrng_s_li, NULL},
		(t_sen){yrng_s_ho, yrng_s_hi, yrng_h_hn}};
	dot.r = map->m[p.y][p.x].s;
	if (!ycheck_dir(dot, map, t, dir))
		return (0);
	return (1);
}

unsigned char			yfuse_pc(t_map *map, t_pc *pc)
{
	t_crd		verif;
	t_crd		cur;

	verif = (t_crd){-1,-1};
	yreset_pc_pos(pc, &pc->it);
	cur = pc->it.hp;
	// il faut free les ares rendus inutiles et malloc ceux qui le deviennent ici.
	while (!yis_coord(cur, &pc->map))
	{
		if (!yfuse_pc_elem((t_crd){cur.x + pc->mic.x, cur.y + pc->mic.y},
			map, pc->map.m[cur.y][cur.x].o))
			return (0);
		yrng_h_hi(&pc->map, cur, &verif);
		if (ycoord_equals(cur, verif))
			ynext_hp(pc->map.m[cur.y][cur.x].o, &pc->map, &cur);
		else
			cur.x += 1;
	}
	yfree_map(&pc->map);
	return (1);
}