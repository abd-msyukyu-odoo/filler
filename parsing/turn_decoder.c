/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   turn_decoder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 12:28:41 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 12:28:42 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	yread_turn3(t_in *in, t_gm *gm)
{
	if (!(in->s = yread((ssize_t)((gm->pc.map.w + 1) * gm->pc.map.h), NULL)))
	{
		yfree_ares(&(gm->map), gm->map.w, gm->map.h);
		yfree_map(&(gm->map));
		return (0);
	}
	if (!ydecode_crop(in, &(gm->pc)))
	{
		yreset_in(in);
		yfree_ares(&(gm->map), gm->map.w, gm->map.h);
		yfree_map(&(gm->map));
		return (0);
	}
	if (!ydecode_pc(in, &(gm->pc), gm->me.o))
	{
		yreset_in(in);
		yfree_map(&(gm->pc.map));
		yfree_ares(&(gm->map), gm->map.w, gm->map.h);
		yfree_map(&(gm->map));
		return (0);
	}
	yreset_in(in);
	return (1);
}

static unsigned char	yread_turn2(t_in *in, t_gm *gm)
{
	if (!ydecode_map(in, &(gm->map)))
	{
		yreset_in(in);
		yfree_map(&(gm->map));
		return (0);
	}
	yreset_in(in);
	if (!(in->s = yread(0, "\n")))
	{
		yfree_ares(&(gm->map), gm->map.w, gm->map.h);
		yfree_map(&(gm->map));
		return (0);
	}
	if (!ydecode_size(in, &(gm->pc.map.w), &(gm->pc.map.h), PIECE) ||
		gm->pc.map.w == 0 || gm->pc.map.h == 0)
	{
		yreset_in(in);
		yfree_ares(&(gm->map), gm->map.w, gm->map.h);
		yfree_map(&(gm->map));
		return (0);
	}
	yreset_in(in);
	return (yread_turn3(in, gm));
}

unsigned char			yread_turn(t_in *in, t_gm *gm)
{
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
	if (!(in->s = yread(0, "\n")))
	{
		yfree_map(&(gm->map));
		return (0);
	}
	yreset_in(in);
	if (!(in->s = yread(ymap_read_size(&(gm->map)), NULL)))
	{
		yfree_map(&(gm->map));
		return (0);
	}
	return (yread_turn2(in, gm));
}
