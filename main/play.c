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

static t_crd			yfind_nearest(t_gm *gm, t_crd o, char t)
{
	int					size;
	t_crd				out;

    if (!yis_coord(o, &gm->map) || (gm->me.o != t && gm->en.o == '\0'))
		return ((t_crd){-1, -1});
	if (gm->map.m[o.y][o.x].o == t)
		return (o);
	size = 1;
	//fprintf(stderr, "\nsize : %d\n", size);
	while (!yis_coord((out = ysonar(gm, o, t, size)), &gm->map) &&
		size < gm->map.h + gm->map.w)
	{
		size++;
		//fprintf(stderr, "\nsize : %d\n", size);
	}
	return (out);
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
	unsigned char (*yrng_o)(t_map*, t_crd, t_crd*))
{
	t_crd 	n;

	if (yrng_o(map, o, &n) && map->m[n.y][n.x].o == t)
		return (1);
	return (0);
}

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

static unsigned char	yenemy_on_sight(t_gm *gm)
{
	t_pc_nav	nav;

	nav.m_o = (t_crd){gm->map.a.x - gm->pc.map.a.x,
		gm->map.a.y - gm->pc.map.a.y};
	yreset_pc_pos(&gm->pc, &nav.it);
	while (ynext_pc_pos(&gm->me, &gm->pc, &nav.it, &nav.p))
	{
		nav.m_p = (t_crd){nav.m_o.x + nav.p.x, nav.m_o.y + nav.p.y};
		if (ycoord_equals(nav.p, gm->pc.map.a))
		{
			if (yseek_target_from_anchor(nav.m_p, gm->en.o, &gm->map))
				return (1);
		}
		else if (yseek_target_from(nav.m_p, gm->en.o, &gm->map))
			return (1);
	}
	return (0);
}

unsigned char			yplay(t_gm *gm)
{
	t_crd			best;
	t_crd			best_map;
	int				score;
	int				cur_score;
	unsigned char	on_sight;
	unsigned char	cur_on_sight;

	best = (t_crd){-1, -1};
	score = -1;
	on_sight = 0;
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
					if (score == -1 || cur_score < score)
					{
						score = cur_score;
						best = gm->pc.map.a;
						best_map = gm->map.a;
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