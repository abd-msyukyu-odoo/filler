/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   are_support.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 12:15:31 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 12:15:33 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

void					yappend_range(t_rng **dst, t_rng *src)
{
	*dst = src;
	(*dst)->d += 1;
}

unsigned char			ymalloc_range(t_rng **rng, int x, int y)
{
	if (!(*rng = (t_rng*)malloc(sizeof(t_rng))))
		return (0);
	(*rng)->s = (t_crd){x, y};
	(*rng)->d = 1;
	return (1);
}
