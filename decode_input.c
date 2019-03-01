/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 18:26:59 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/01 19:03:39 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned int		ft_atoui_limited(const char *str, size_t s, size_t e)
{
	unsigned int		result;
	size_t				i;
	unsigned int		tmp;

	result = 0;
	i = s;
	while (i < e && str[i] >= '0' && str[i] <= '9')
	{
		tmp = result;
		result = result * 10 + str[i++] - '0';
		if (result < tmp)
			return (~((unsigned int)0));
	}
	return (result);
}

static size_t			decode_size(char *in, int *w, int *h, size_t i)
{
	static char			*pl = "Plateau ";
	static char			*pi = "Piece ";

	
}

unsigned char			decode_input(char *in, MAP *map)
{
	if (!decode_size(in, &(map->w), &(map->h), 0))
		return (0);
}
