/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piece_cropper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 12:25:14 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 12:25:15 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static void				yinit_mic_mac(t_pc *pc)
{
	pc->mic = (t_crd){-1, -1};
	pc->mac = (t_crd){-1, -1};
}

unsigned char			ydecode_crop(t_in *in, t_pc *pc)
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
		while (++x < pc->map.w && yvalid_for_piece(in->s[l]))
		{
			if (in->s[l] == '*')
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
