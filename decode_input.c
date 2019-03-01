/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 18:26:59 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/01 20:59:09 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned int		ft_atoui_limited(STR *in)
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

static unsigned char	decode_size(STR *in, int *w, int *h, char *ref)
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

static unsigned char	ignore_line(STR *in)
{
	while (in->s[in->p] != '\0' && in->s[in->p] != '\n')
		in->p++;
	if (in->s[in->p] == '\0')
		return (0);
	in->p++;
	return (1);
}

static unsigned char	decode_line(STR *in, MAP *map)
{
	
}

static unsigned char	ignore_prefix(STR *in)
{

}

static void				free_ares(MAP *map, size_t row)
{
	
}

static unsigned char	decode_map(STR *in, MAP *map)
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
		if (!decode_line(in, map))
		{
			while (y-- > 0)
				free_ares(map, y);
			return (0);
		}
	}
	return (1);
}

static unsigned char	malloc_map(MAP *map)
{
	size_t		y;

	map->m = (ARE**)malloc(sizeof(ARE*) * map->h);
	if (!map->m)
		return (0);
	y = 0;
	while (y < map->h)
	{
		map->m[y] = (ARE*)malloc(sizeof(ARE) * map->w);
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

static void				free_map(MAP *map)
{
	size_t		y;

	y = 0;
	while (y < map->h)
		free(map->m[y++]);
	free(map->m);
}

unsigned char			decode_input(STR in, MAP *map)
{
	static char			*pl = "Plateau ";
	static char			*pi = "Piece ";

	if (!decode_size(&in, &(map->w), &(map->h), pl))
		return (0);
	if (!malloc_map(map))
		return (0);
	//a partir d'ici : free la map
	if (!ignore_line(&in))
		return (0);
	if (!decode_map(&in, map))
		return (0);
	//a partir d'ici : free les AREs avant de free la map
	printf("%d\n%d\n", map->w, map->h);
	return (1);
}
