/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 18:26:59 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/05 14:53:08 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned int		yatoui_limited(t_str *in)
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

static unsigned char	ydecode_size(t_str *in, int *w, int *h, char *ref)
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

static unsigned char	yignore_line(t_str *in)
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

static void				ydereference_rng(t_map *map, size_t x, size_t y,
		t_rng *rng)
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

static void				yfree_ares(t_map *map, size_t x, size_t y)
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
	(*dst)->e += 1;
}

static unsigned char	ymalloc_range(t_rng **rng)
{
	if (!(*rng = (t_rng*)malloc(sizeof(t_rng))))
		return (0);
	(*rng)->s = 0;
	(*rng)->e = 0;
	return (1);
}

static unsigned char	yadd_are_vh(size_t x, size_t y, t_map *map)
{
	if (x == 0)
	{
		if (!ymalloc_range(&(map->m[y][x].h)))
			return (0);
	}
	else
		yappend_range(&(map->m[y][x].h), map->m[y][x - 1].h);
	if (y == 0)
	{
		if (!ymalloc_range(&(map->m[y][x].v)))
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

static unsigned char	yabort_are_bs(size_t x, size_t y, t_map *map)
{
	if (x == 0)
		free(map->m[y][x].h);
	if (y == 0)
		free(map->m[y][x].v);
	if (x == map->w - 1 && y == 0)
		free(map->m[y][x].b);
	return (0);
}

static unsigned char	yadd_are_bs(size_t x, size_t y, t_map *map)
{
	if (x == 0 || y == 0)
	{
		if (!ymalloc_range(&(map->m[y][x].b)))
			return (yabort_are_bs(x, y, map));
	}
	else
		yappend_range(&(map->m[y][x].b), map->m[y - 1][x - 1].b);
	if (x == map->w - 1 || y == 0)
	{
		if (!ymalloc_range(&(map->m[y][x].s)))
			return (yabort_are_bs(x, y, map));
	}
	else
		yappend_range(&(map->m[y][x].s), map->m[y - 1][x + 1].s);
	return (1);
}

static unsigned char	yadd_are(char o, size_t x, size_t y, t_map *map)
{
	map->m[y][x].o = o;
	if (!yadd_are_vh(x, y, map))
		return (0);
	if (!yadd_are_bs(x, y, map))
		return (0);
	return (1);
}

static unsigned char	ydecode_line(t_str *in, t_map *map, size_t y,
		unsigned char (*f)(char))
{
	size_t			x;

	x = 0;
	while (x < map->w && f(in->s[in->p]))
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

static unsigned char	yignore_prefix(t_str *in)
{
	while (in->s[in->p] >= '0' && in->s[in->p] <= '9')
		in->p++;
	if (in->s[in->p++] != ' ')
		return (0);
	return (1);
}

static unsigned char	ydecode_map(t_str *in, t_map *map)
{
	size_t		y;

	y = 0;
	while (y < map->h)
	{
		if (!yignore_prefix(in))
			return (0);
		if (!ydecode_line(in, map, y, yvalid_for_map))
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
	size_t		y;

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

static void				yfree_map(t_map *map)
{
	size_t		y;

	y = 0;
	while (y < map->h)
		free(map->m[y++]);
	free(map->m);
}

static unsigned char	ydecode_pc(t_str *in, t_pc *pc)
{
	size_t		y;

	in->p += (pc->map.w + 1) * pc->mic.y;
	printf("%lu\n", in->p);
	pc->map = (t_map){pc->mac.x + 1 - pc->mic.x, pc->mac.y + 1 - pc->mic.y,
		NULL};
	if (!ymalloc_map(&(pc->map)))
		return (0);
	y = 0;
	while (y < pc->map.h)
	{
		in->p += pc->mic.x;
		if (!ydecode_line(in, &(pc->map), y, yvalid_for_piece))
		{
			if (y > 0)
				yfree_ares(&(pc->map), pc->map.w, y);
			return (0);
		}
		while (in->s[in->p++] != '\n')
			;
		++y;
	}
	return (0);
}

static unsigned char	ydecode_crop(t_str *in, t_pc *pc)
{
	size_t		y;
	size_t		x;
	size_t		l;

	l = in->p;
	y = -1;
	while (++y < pc->map.h)
	{
		x = -1;
		while (++x < pc->map.w && yvalid_for_piece(in->s[l]))
		{
			if (in->s[l] == '*')
			{
				pc->mic.x = (x < pc->mic.x) ? x : pc->mic.x;
				pc->mic.y = (y < pc->mic.y) ? y : pc->mic.y;
				pc->mac.x = (x > pc->mac.x) ? x : pc->mac.x;
				pc->mac.y = (y > pc->mac.y) ? y : pc->mac.y;
			}
			l++;
		}
		if (in->s[l++] != '\n')
			return (0);
	}
	return (1);
}

unsigned char			ydecode_input(t_str in, t_map *map, t_pc *pc)
{
	static char			*pl = "Plateau ";
	static char			*pi = "Piece ";

	if (!ydecode_size(&in, &(map->w), &(map->h), pl))
		return (0);
	if (map->w == 0 || map->h == 0 || !ymalloc_map(map))
		return (0);
	//+ free map
	if (!yignore_line(&in) || !ydecode_map(&in, map))
	{
		yfree_map(map);
		return (0);
	}
	//+ free ares
	
	if (!ydecode_size(&in, &(pc->map.w), &(pc->map.h), pi))
	{
		yfree_ares(map, map->w, map->h);
		yfree_map(map);
		return (0);
	}
	if (pc->map.w == 0 || pc->map.h == 0 || !ydecode_crop(&in, pc))
	{
		yfree_ares(map, map->w, map->h);
		yfree_map(map);
		return (0);
	}
	if (!ydecode_pc(&in, pc))
	{
		yfree_map(&(pc->map));
		yfree_ares(map, map->w, map->h);
		yfree_map(map);
		return (0);
	}

	return (1);
}
