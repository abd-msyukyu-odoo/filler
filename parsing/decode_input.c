/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 18:26:59 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/11 14:38:57 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

unsigned int		yatoui_limited(t_in *in)
{
	unsigned int		result;
	unsigned int		tmp;

	result = 0;
	while (in->s[in->p] >= '0' && in->s[in->p] <= '9')
	{
		tmp = result;
		result = result * 10 + in->s[in->p++] - '0';
		if (result < tmp)
			return (~((unsigned int)0));
	}
	return (result);
}

static unsigned char	ydecode_size(t_in *in, int *w, int *h, char *ref)
{
	size_t				p;

	p = 0;
	while (in->s[p + in->p] == ref[p])
		p++;
	if (ref[p] != '\0')
		return (0);
	in->p += p;
	*h = yatoui_limited(in);
	if (in->s[in->p++] != ' ')
		return (0);
	*w = yatoui_limited(in);
	if (in->s[in->p++] != ':')
		return (0);
	if (in->s[in->p++] != '\n')
		return (0);
	return (1);
}

static unsigned char	yvalid_for_map(char c)
{
	return (c == '.' || c == 'O' || c == 'X' || c == 'o' || c == 'x');
}

static unsigned char	yvalid_for_piece(char c)
{
	return (c == '.' || c == '*');
}

static unsigned char	yignore_line(t_in *in)
{
	while (in->s[in->p] != '\0' && in->s[in->p] != '\n')
		in->p++;
	if (in->s[in->p] == '\0')
		return (0);
	in->p++;
	return (1);
}

static char				yc_to_upper(char c)
{
	return (((c >= 'a' && c <= 'z') ? 'A' - 'a' : 0) + c);
}

static void				ydereference_rng(t_map *map, int x, int y, t_rng *rng)
{
	if (!rng)
		return ;
	while (y-- > 0)
	{
		while (x-- > 0)
		{
			if (map->m[y][x].h == rng)
				map->m[y][x].h = NULL;
			if (map->m[y][x].v == rng)
				map->m[y][x].v = NULL;
			if (map->m[y][x].b == rng)
				map->m[y][x].b = NULL;
			if (map->m[y][x].s == rng)
				map->m[y][x].s = NULL;
		}
		x = map->w;
	}
	free(rng);
}

static void				yfree_ares(t_map *map, int x, int y)
{
	while (y-- > 0)
	{
		while (x-- > 0)
		{
			ydereference_rng(map, x, y + 1, map->m[y][x].v);
			ydereference_rng(map, x, y + 1, map->m[y][x].h);
			ydereference_rng(map, x, y + 1, map->m[y][x].b);
			ydereference_rng(map, x, y + 1, map->m[y][x].s);
		}
		x = map->w;
	}
}

static void				yappend_range(t_rng **dst, t_rng *src)
{
	*dst = src;
	(*dst)->d += 1;
}

static unsigned char	ymalloc_range(t_rng **rng, int x, int y)
{
	if (!(*rng = (t_rng*)malloc(sizeof(t_rng))))
		return (0);
	(*rng)->s = (t_crd){x, y};
	(*rng)->d = 1;
	return (1);
}

static unsigned char	yadd_are_vh(int x, int y, t_map *map)
{
	if (x == 0 || map->m[y][x].o != map->m[y][x - 1].o)
	{
		if (!ymalloc_range(&(map->m[y][x].h), x, y))
			return (0);
	}
	else
		yappend_range(&(map->m[y][x].h), map->m[y][x - 1].h);
	if (y == 0 || map->m[y][x].o != map->m[y - 1][x].o)
	{
		if (!ymalloc_range(&(map->m[y][x].v), x, y))
		{
			if (x == 0)
				free(map->m[y][x].h);
			return (0);
		}
	}
	else
		yappend_range(&(map->m[y][x].v), map->m[y - 1][x].v);
	return (1);
}

static unsigned char	yabort_are_bs(int x, int y, t_map *map)
{
	if (x == 0)
		free(map->m[y][x].h);
	if (y == 0)
		free(map->m[y][x].v);
	if (x == map->w - 1 && y == 0)
		free(map->m[y][x].b);
	return (0);
}

static unsigned char	yadd_are_bs(int x, int y, t_map *map)
{
	if (x == 0 || y == 0 || map->m[y][x].o != map->m[y - 1][x - 1].o)
	{
		if (!ymalloc_range(&(map->m[y][x].b), x, y))
			return (yabort_are_bs(x, y, map));
	}
	else
		yappend_range(&(map->m[y][x].b), map->m[y - 1][x - 1].b);
	if (x == map->w - 1 || y == 0 || map->m[y][x].o != map->m[y - 1][x + 1].o)
	{
		if (!ymalloc_range(&(map->m[y][x].s), x, y))
			return (yabort_are_bs(x, y, map));
	}
	else
	{
		yappend_range(&(map->m[y][x].s), map->m[y - 1][x + 1].s);
		map->m[y][x].s->s = (t_crd){x, y};
	}
	return (1);
}

static unsigned char	yadd_are(char o, int x, int y, t_map *map)
{
	map->m[y][x].o = o;
	if (!yadd_are_vh(x, y, map))
		return (0);
	if (!yadd_are_bs(x, y, map))
		return (0);
	return (1);
}

static unsigned char	ydecode_line_map(t_in *in, t_map *map, int y)
{
	int				x;

	x = 0;
	while (x < map->w && yvalid_for_map(in->s[in->p]))
	{
		if (!yadd_are(yc_to_upper(in->s[in->p]), x, y, map))
		{
			if (x > 0)
				yfree_ares(map, x, y + 1);
			return (0);
		}
		x++;
		in->p++;
	}
	if (x < map->w || in->s[in->p++] != '\n')
	{
		if (x > 0)
			yfree_ares(map, x, y + 1);
		return (0);
	}
	return (1);
}

static unsigned char	ydecode_line_pc(t_in *in, t_map *map, int y, char o, char t)
{
	int				x;

	x = 0;
	while (x < map->w) //&& yvalid_for_piece(in->s[in->p])) // deja verifie
	{
		in->s[in->p] = (in->s[in->p] == t) ? o : in->s[in->p];
		if (!yadd_are(in->s[in->p], x, y, map))
		{
			if (x > 0)
				yfree_ares(map, x, x + 1);
			return (0);
		}
		x++;
		in->p++;
	}
	return (1);
}

static unsigned char	yignore_prefix(t_in *in)
{
	while (in->s[in->p] >= '0' && in->s[in->p] <= '9')
		in->p++;
	if (in->s[in->p++] != ' ')
		return (0);
	return (1);
}

static unsigned char	ydecode_map(t_in *in, t_map *map)
{
	int			y;

	y = 0;
	while (y < map->h)
	{
		if (!yignore_prefix(in))
			return (0);
		if (!ydecode_line_map(in, map, y))
		{
			if (y > 0)
				yfree_ares(map, map->w, y);
			return (0);
		}
		y++;
	}
	return (1);
}

static unsigned char	ymalloc_map(t_map *map)
{
	int			y;

	map->m = (t_are**)malloc(sizeof(t_are*) * map->h);
	if (!map->m)
		return (0);
	y = 0;
	while (y < map->h)
	{
		map->m[y] = (t_are*)malloc(sizeof(t_are) * map->w);
		if (!map->m[y])
		{
			while (y > 0)
				free(map->m[--y]);
			free(map->m);
			return (0);
		}
		++y;
	}
	return (1);
}

static void				yfree_m(t_map *map)
{
	int			y;

	y = 0;
	while (y < map->h)
		free(map->m[y++]);
	free(map->m);
}

static unsigned char	ydecode_pc(t_in *in, t_pc *pc, char o, char t)
{
	int			y;

	in->p += (pc->map.w + 1) * pc->mic.y;
	pc->map = (t_map){pc->mac.x + 1 - pc->mic.x, pc->mac.y + 1 - pc->mic.y,
		NULL, (t_crd){-1, -1}};
	if (!ymalloc_map(&(pc->map)))
		return (0);
	y = 0;
	while (y < pc->map.h)
	{
		in->p += pc->mic.x;
		if (!ydecode_line_pc(in, &(pc->map), y, o, t))
		{
			if (y > 0)
				yfree_ares(&(pc->map), pc->map.w, y);
			return (0);
		}
		while (in->s[in->p++] != '\n')
			;
		++y;
	}
	return (1);
}

static void				yinit_mic_mac(t_pc *pc)
{
	pc->mic = (t_crd){-1, -1};
	pc->mac = (t_crd){-1, -1};
}

static unsigned char	ydecode_crop(t_in *in, t_pc *pc, char t,
	unsigned char (*valid)(char))
{
	int			y;
	int			x;
	size_t		l;

	l = in->p;
	yinit_mic_mac(pc);
	y = -1;
	while (++y < pc->map.h)
	{
		x = -1;
		while (++x < pc->map.w && valid(in->s[l]))
		{
			if (in->s[l] == t)
			{
				pc->mic.x = (pc->mic.x == -1 || x < pc->mic.x) ? x : pc->mic.x;
				pc->mic.y = (pc->mic.y == -1 || y < pc->mic.y) ? y : pc->mic.y;
				pc->mac.x = (pc->mac.x == -1 || x > pc->mac.x) ? x : pc->mac.x;
				pc->mac.y = (pc->mac.y == -1 || y > pc->mac.y) ? y : pc->mac.y;
			}
			l++;
		}
		if (in->s[l++] != '\n')
			return (0);
	}
	return (1);
}

void					yfree_pc(t_pc *pc)
{
	yfree_ares(&(pc->map), pc->map.w, pc->map.h);
	yfree_m(&(pc->map));
}

void					yfree_map(t_map *map)
{
	yfree_ares(map, map->w, map->h);
	yfree_m(map);
}

void					yfree_turn(t_map *map, t_pc *pc)
{
	yfree_pc(pc);
	yfree_map(map);
}

/*
unsigned char			ydecode_input(t_in in, t_map *map, t_pc *pc, char o)
{
	if (!ydecode_size(&in, &(map->w), &(map->h), PLATEAU))
		return (0);
	if (map->w == 0 || map->h == 0 || !ymalloc_map(map))
		return (0);
	if (!yignore_line(&in) || !ydecode_map(&in, map))
	{
		yfree_map(map);
		return (0);
	}
	if (!ydecode_size(&in, &(pc->map.w), &(pc->map.h), PIECE) ||
		pc->map.w == 0 || pc->map.h == 0 || !ydecode_crop(&in, pc))
	{
		yfree_ares(map, map->w, map->h);
		yfree_map(map);
		return (0);
	}
	if (!ydecode_pc(&in, pc, o))
	{
		yfree_map(&(pc->map));
		yfree_ares(map, map->w, map->h);
		yfree_map(map);
		return (0);
	}
	return (1);
}
*/

static void				yreset_in(t_in *in)
{
	free(in->s);
	in->p = 0;
}

static ssize_t			ymap_read_size(t_map *map)
{
	int				i;
	size_t			ref;
	int				info;
	ssize_t			res;

	ref = 1000;
	i = 0;
	info = 3;
	res = 0;
	while (i < map->h)
	{
		if ((size_t)i == ref)
		{
			info++;
			ref *= 10;
		}
		res += (ssize_t)info;
		i++;
	}
	return ((ssize_t)map->h * (2 + (ssize_t)map->w) + res);
}

static unsigned char	yread_turn3(t_in *in, t_gm *gm)
{
	//map piece
	if (!(in->s = yread((ssize_t)((gm->pc.map.w + 1) * gm->pc.map.h), NULL)))
	{
		yfree_map(&(gm->map));
		return (0);
	}
	if (!ydecode_crop(in, &(gm->pc), '*', yvalid_for_piece))
	{
		yreset_in(in);
		yfree_map(&(gm->map));
		return (0);
	}
	if (!ydecode_pc(in, &(gm->pc), gm->me.o, '*'))
	{
		yreset_in(in);
		yfree_m(&(gm->pc.map));
		yfree_map(&(gm->map));
		return (0);
	}
	yreset_in(in);
	return (1);
}

static unsigned char	yread_turn2(t_in *in, t_gm *gm)
{
	//piece
	if (!(in->s = yread(0, "\n")))
	{
		yfree_map(&(gm->map));
		return (0);
	}
	if (!ydecode_size(in, &(gm->pc.map.w), &(gm->pc.map.h), PIECE) ||
		gm->pc.map.w == 0 || gm->pc.map.h == 0)
	{
		yreset_in(in);
		yfree_map(&(gm->map));
		return (0);
	}
	yreset_in(in);
	return (yread_turn3(in, gm));
}

static unsigned char	yread_turn1(t_in *in, t_gm *gm)
{
	//pateau
	if (!(in->s = yread(0, "\n")))
		return (0);
	if (!ydecode_size(in, &(gm->map.w), &(gm->map.h), PLATEAU))
	{
		yreset_in(in);
		return (0);
	}
	if (gm->map.w == 0 || gm->map.h == 0 || !ymalloc_map(&(gm->map)))
	{
		yreset_in(in);
		return (0);
	}
	yreset_in(in);
	//ligne inutile
	if (!(in->s = yread(0, "\n")))
	{
		yfree_m(&(gm->map));
		return (0);
	}
	yreset_in(in);
	//map plateau
	if (!(in->s = yread(ymap_read_size(&(gm->map)), NULL)))
	{
		yfree_m(&(gm->map));
		return (0);
	}
	if (!ydecode_map(in, &(gm->map)))
	{
		yreset_in(in);
		yfree_m(&(gm->map));
		return (0);
	}
	yreset_in(in);
	return (yread_turn2(in, gm));
}

static unsigned char	yread_turn_lite1(t_in *in, t_gm *gm)
{
	t_pc		en_pc;
	//plateau
	if (!(in->s = yread(0, "\n")))
	{
		yfree_map(&gm->map);
		return (0);
	}
	yreset_in(in);
	//ligne inutile
	if (!(in->s = yread(0, "\n")))
	{
		yfree_map(&gm->map);
		return (0);
	}
	yreset_in(in);
	//map (read)
	if (!(in->s = yread(ymap_read_size(&(gm->map)), NULL)))
	{
		yfree_map(&(gm->map));
		return (0);
	}
	//piece (nouvel ennemi sur map)
	en_pc.map.w = gm->map.w;
	en_pc.map.h = gm->map.h;
	if (!ydecode_crop(in, &en_pc, ft_tolower(gm->en.o), yvalid_for_map))
	{
		yreset_in(in);
		yfree_map(&(gm->map));
		return (0);
	}
	if (!ydecode_pc(in, &en_pc, ft_tolower(gm->en.o), ft_tolower(gm->en.o)))
	{
		yreset_in(in);
		yfree_m(&(en_pc.map));
		yfree_map(&(gm->map));
		return (0);
	}
	yreset_in(in);
	if (!yfuse_pc(&gm->map, &en_pc))
	{
		yfree_map(&(en_pc.map));
		yfree_map(&(gm->map));
		return (0);
	}
	return (yread_turn2(in, gm));
}

unsigned char			yread_turn(t_in *in, t_gm *gm)
{
	//return yread_turn1(in, gm);
	
	if (gm->map.m == NULL)
		return yread_turn1(in, gm);
	else
		return yread_turn_lite1(in, gm);
}