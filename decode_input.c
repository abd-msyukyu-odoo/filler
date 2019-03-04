/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 18:26:59 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/04 14:43:16 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned int		ft_atoui_limited(t_str *in)
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

static unsigned char	decode_size(t_str *in, int *w, int *h, char *ref)
{
	size_t				p;

	p = 0;
	while (in->s[p + in->p] == ref[p])
		p++;
	if (ref[p] != '\0')
		return (0);
	in->p += p;
	*h = ft_atoui_limited(in);
	if (in->s[in->p++] != ' ')
		return (0);
	*w = ft_atoui_limited(in);
	if (in->s[in->p++] != ':')
		return (0);
	if (in->s[in->p++] != '\n')
		return (0);
	return (1);
}

static unsigned char	valid_for_map(char c)
{
	return (c == '.' || c == 'O' || c == 'X' || c == 'o' || c == 'x');
}

static unsigned char	valid_for_piece(char c)
{
	return (c == '.' || c == '*');
}

static unsigned char	ignore_line(t_str *in)
{
	while (in->s[in->p] != '\0' && in->s[in->p] != '\n')
		in->p++;
	if (in->s[in->p] == '\0')
		return (0);
	in->p++;
	return (1);
}

static char				c_to_upper(char c)
{
	return (((c >= 'a' && c <= 'z') ? 'A' - 'a' : 0) + c);
}

static void				dereference_rng(t_map *map, size_t x, size_t y,
		t_rng *rng)
{
	if (!rng)
		return ;
	while (y-- > 0)
	{
		while (x-- > 0)
		{
			if ((*map->m)[y][x].h == rng)
				(*map->m)[y][x].h = NULL;
			if ((*map->m)[y][x].v == rng)
				(*map->m)[y][x].v = NULL;
			if ((*map->m)[y][x].b == rng)
				(*map->m)[y][x].b = NULL;
			if ((*map->m)[y][x].s == rng)
				(*map->m)[y][x].s = NULL;
		}
		x = map->w;
	}
	free(rng);
}

static void				free_ares(t_map *map, size_t x, size_t y)
{
	while (y-- > 0)
	{
		while (x-- > 0)
		{
			printf("%lu%lu\n", y, x);
			dereference_rng(map, x, y, (*map->m)[y][x].v);
			dereference_rng(map, x, y, (*map->m)[y][x].h);
			dereference_rng(map, x, y, (*map->m)[y][x].b);
			dereference_rng(map, x, y, (*map->m)[y][x].s);
		}
		x = map->w;
	}
}

static void				append_range(t_rng **dst, t_rng *src)
{
	*dst = src;
	(*dst)->e += 1;
}

static unsigned char	malloc_range(t_rng **rng)
{
	if (!(*rng = (t_rng*)malloc(sizeof(t_rng))))
		return (0);
	(*rng)->s = 0;
	(*rng)->e = 0;
	return (1);
}

static unsigned char	add_are_vh(size_t x, size_t y, t_map *map)
{
	if (x == 0)
	{
		if (!malloc_range(&((*map->m)[y][x].h)))
			return (0);
	}
	else
		append_range(&((*map->m)[y][x].h), (*map->m)[y][x - 1].h);
	if (y == 0)
	{
		if (!malloc_range(&((*map->m)[y][x].v)))
		{
			if (x == 0)
				free((*map->m)[y][x].h);
			return (0);
		}
	}
	else
		append_range(&((*map->m)[y][x].v), (*map->m)[y - 1][x].v);
	return (1);
}

static unsigned char	abort_are_bs(size_t x, size_t y, t_map *map)
{
	if (x == 0)
		free((*map->m)[y][x].h);
	if (y == 0)
		free((*map->m)[y][x].v);
	if (x == map->w - 1 && y == 0)
		free((*map->m)[y][x].b);
	return (0);
}

static unsigned char	add_are_bs(size_t x, size_t y, t_map *map)
{
	if (x == 0 || y == 0)
	{
		if (!malloc_range(&((*map->m)[y][x].b)))
			return (abort_are_bs(x, y, map));
	}
	else
		append_range(&((*map->m)[y][x].b), (*map->m)[y - 1][x - 1].b);
	if (x == map->w - 1 || y == 0)
	{
		if (!malloc_range(&((*map->m)[y][x].s)))
			return (abort_are_bs(x, y, map));
	}
	else
		append_range(&((*map->m)[y][x].s), (*map->m)[y - 1][x + 1].s);
	return (1);
}

static unsigned char	add_are(char o, size_t x, size_t y, t_map *map)
{
	(*map->m)[y][x].o = o;
	printf("%lu-%lu\n", x, y);
	if (!add_are_vh(x, y, map))
		return (0);
	if (!add_are_bs(x, y, map))
		return (0);
	return (1);
}

static unsigned char	decode_line(t_str *in, t_map *map, size_t y)
{
	size_t			x;

	x = 0;
	while (x < map->w && valid_for_map(in->s[in->p]))
	{
		if (!add_are(c_to_upper(in->s[in->p]), x, y, map))
		{
			if (x > 0)
				free_ares(map, x, y + 1);
			return (0);
		}
		x++;
		in->p++;
	}
	if (x < map->w || in->s[in->p++] != '\n')
	{
		if (x > 0)
			free_ares(map, x, y + 1);
		return (0);
	}
	return (1);
}

static unsigned char	ignore_prefix(t_str *in)
{
	while (in->s[in->p] >= '0' && in->s[in->p] <= '9')
		in->p++;
	if (in->s[in->p++] != ' ')
		return (0);
	return (1);
}

static unsigned char	decode_map(t_str *in, t_map *map)
{
	//do h times
		//ignore prefix
			//stop if not a digit
		//read w map_values then \n
			//stop at the first invalid char
	size_t		y;

	y = 0;
	while (y < map->h)
	{
		if (!ignore_prefix(in))
			return (0);
		if (!decode_line(in, map, y))
		{
			if (y > 0)
				free_ares(map, map->w, y);
			return (0);
		}
		y++;
	}
	return (1);
}

static unsigned char	malloc_map(t_map *map)
{
	size_t		y;
	t_are		**m;

	printf("%lu\n%lu\n%lu\n%lu\n", sizeof(t_are), sizeof(map),
			sizeof(*map), sizeof(t_are***));
	printf("%p\n", map);
	m = (t_are**)malloc(sizeof(t_are*) * map->h);
	map->m = &m;
	if (!(*map->m))
		return (0);
	y = 0;
	while (y < map->h)
	{
		(*map->m)[y] = (t_are*)malloc(sizeof(t_are) * map->w);
		if (!(*map->m)[y])
		{
			while (y > 0)
				free((*map->m)[--y]);
			free((*map->m));
			return (0);
		}
		size_t x = 0;
		while (x < map->w)
		{
			(*map->m)[y][x] = (t_are){NULL, NULL, NULL, NULL, '\0'};
			x++;
		}
		++y;
	}
	return (1);
}

static void				free_map(t_map *map)
{
	size_t		y;

	y = 0;
	while (y < map->h)
		free((*map->m)[y++]);
	free((*map->m));
}

unsigned char			decode_input(t_str in, t_map *map)
{
	static char			*pl = "Plateau ";
	static char			*pi = "Piece ";

	if (!decode_size(&in, &(map->w), &(map->h), pl))
		return (0);
	printf("\n%d\n%d\n", map->w, map->h);
	if (map->w == 0 || map->h == 0 || !malloc_map(map))
		return (0);
	//a partir d'ici : free la map
	if (!ignore_line(&in) || !decode_map(&in, map))
	{
		free_map(map);
		return (0);
	}

	size_t		x, y;
	y = 0;
	while (y < map->h)
	{
		x = 0;
		while (x < map->w)
		{
			printf("%lu\n", x);
			printf("%p\n", &(*map->m)[y][x]);
			//write(1, &((*map->m)[y][x].o), 1);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
	printf("coucou\n");
	//tbc
	//free_ares(map, map->w, map->h);
	//a partir d'ici : free les t_ares avant de free la map
	return (1);
}
