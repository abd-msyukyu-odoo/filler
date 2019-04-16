/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 16:25:26 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/13 13:55:00 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

int				main(void)
{
	t_in				in;
	t_gm				gm;
	unsigned char		played;

	if (!(in.s = yread(0, "\n")))
		return (0);
	in.p = 0;
	if (!ydecode_player(in, &gm.me, &gm.en))
	{
		free(in.s);
		return (0);
	}
	free(in.s);
	in.p = 0;
	while (yread_turn(&in, &gm))
	{
		played = yplay(&gm);
		yfree_turn(&gm.map, &gm.pc);
		in.p = 0;
		if (!played)
			return (0);
	}
	return (0);
}
