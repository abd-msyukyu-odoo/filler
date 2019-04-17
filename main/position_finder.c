/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   position_finder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:45:56 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 13:45:57 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned char	yidentify_pos_vp(t_gm *gm, t_crd in, t_crd *mevp,
	t_crd *envp)
{
	if (!yis_coord(*envp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->en.o)
		*envp = in;
	else if (!yis_coord(*mevp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->me.o)
		*mevp = in;
	if (yis_coord(*envp, &(gm->map)) && yis_coord(*mevp, &(gm->map)))
		return (1);
	return (0);
}

static unsigned char	yfind_start_vp(t_gm *gm, t_crd *mevp, t_crd *envp)
{
	t_crd		in;

	in = (t_crd){0, 0};
	*mevp = (t_crd){-1, -1};
	*envp = (t_crd){-1, -1};
	while (in.x < gm->map.w)
	{
		if (yidentify_pos_vp(gm, in, mevp, envp))
			return (1);
		while (yrng_v_ho(&(gm->map), in, &(in)))
			if (yidentify_pos_vp(gm, in, mevp, envp))
				return (1);
		in = (t_crd){in.x + 1, 0};
	}
	gm->en.o = '\0';
	if (!yis_coord(*mevp, &(gm->map)))
		return (0);
	return (1);
}

static unsigned char	yidentify_pos_hp(t_gm *gm, t_crd in, t_crd *mehp,
	t_crd *enhp)
{
	if (!yis_coord(*enhp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->en.o)
		*enhp = in;
	else if (!yis_coord(*mehp, &(gm->map)) &&
			gm->map.m[in.y][in.x].o == gm->me.o)
		*mehp = in;
	if (yis_coord(*enhp, &(gm->map)) && yis_coord(*mehp, &(gm->map)))
		return (1);
	return (0);
}

static unsigned char	yfind_start_hp(t_gm *gm, t_crd *mehp, t_crd *enhp)
{
	t_crd		in;

	in = (t_crd){0, 0};
	*mehp = (t_crd){-1, -1};
	*enhp = (t_crd){-1, -1};
	while (in.y < gm->map.h)
	{
		if (yidentify_pos_hp(gm, in, mehp, enhp))
			return (1);
		while (yrng_h_ho(&(gm->map), in, &(in)))
			if (yidentify_pos_hp(gm, in, mehp, enhp))
				return (1);
		in = (t_crd){0, in.y + 1};
	}
	gm->en.o = '\0';
	if (!yis_coord(*mehp, &(gm->map)))
		return (0);
	return (1);
}

unsigned char			yfind_start_positions(t_gm *gm)
{
	return (yfind_start_hp(gm, &gm->me.it.hp, &gm->en.it.hp) &&
		yfind_start_vp(gm, &gm->me.it.vp, &gm->en.it.vp));
}
