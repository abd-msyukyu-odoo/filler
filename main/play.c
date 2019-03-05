/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 18:39:13 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/05 19:54:33 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	yis_coord(t_crd *crd)
{
	return (crd->x ! -1 && crd->y != -1);
}

static unsigned char	yfind_owned(t_gm *gm)
{
	int			y;
	int			x;

	gm->me.s = (t_crd){-1, -1};
	gm->en.s = (t_crd){-1, -1};
	y = 0;
	while (y < gm->map.h)
	{
		x = 0;
		while (x < gm->map.w)
		{
			if (!yis_coord(gm->en.s) && gm->map.m[y][x].o == gm->en.o)
				gm->en.s = (t_crd){x, y};
			else if (!yis_coord(gm->me.s) && gm->map.m[y][x].o == gm->me.o)
				gm->me.s = (t_crd){x, y};
			if (yis_coord(gm->en.s) && yis_coord(gm->me.s))
				return (1);
			x++;
		}
		y++;
	}
	gm->en.o = '\0';
	if (!yis_coord(gm->me.s))
		return (0);
	return (1);
}

void					play(t_gm *gm)
{
	//chercher une case de contour du joueur sur la map
	//chercher une case de contour de la piece (attention debordement)
	//tester si la piece peut se mettre
	//a partir d'une piece d'un joueur, on peut facilement trouver toutes les
	//autres
	if (!yfind_owned(gm))
		return ;
	while (ynext_pos(&(gm->me)))
	{
		while (ynext_pc_pos(&(gm->pc))
		{
			if (ycan_put_piece(&(gm->me), &(gm->pc)))
				yput_piece(&(gm->pc.s));
		}
		yreset_pc_pos(&(gm->pc));
	}
}
