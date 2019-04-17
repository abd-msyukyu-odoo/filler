/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:37:14 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 13:37:15 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

unsigned char			yis_coord(t_crd crd, t_map *map)
{
	if (!map)
		return (crd.x > -1 && crd.y > -1);
	return (crd.x > -1 && crd.y > -1 && crd.x < map->w &&
			crd.y < map->h);
}

unsigned char			ycoord_equals(t_crd c1, t_crd c2)
{
	return (c1.x == c2.x && c1.y == c2.y);
}
