/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sonar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 14:01:40 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 14:01:41 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static t_crd			ysonar_backslash(t_gm *gm, char t, t_crd s, t_crd e)
{
	t_crd				d;

	if (!yfit_backslash(gm, &s, &e))
		return ((t_crd){-1, -1});
	d = s;
	if (gm->map.m[d.y][d.x].o == t)
		return (d);
	while (yrng_b_ho(&gm->map, d, &d))
	{
		if (d.x > e.x)
			return ((t_crd){-1, -1});
		if (gm->map.m[d.y][d.x].o == t)
			return (d);
	}
	return ((t_crd){-1, -1});
}

static t_crd			ysonar_slash(t_gm *gm, char t, t_crd s, t_crd e)
{
	t_crd				d;

	if (!yfit_slash(gm, &s, &e))
		return ((t_crd){-1, -1});
	d = s;
	if (gm->map.m[d.y][d.x].o == t)
		return (d);
	while (yrng_s_ho(&gm->map, d, &d))
	{
		if (d.x > e.x)
			return ((t_crd){-1, -1});
		if (gm->map.m[d.y][d.x].o == t)
			return (d);
	}
	return ((t_crd){-1, -1});
}

static t_crd			ysonar_quarter(t_gm *gm, t_crd o, char t, t_crd d)
{
	t_crd				s;
	t_crd				e;

	yidentify_quarter(o, d, &s, &e);
	if (s.y < e.y)
		return (ysonar_backslash(gm, t, s, e));
	else
		return (ysonar_slash(gm, t, s, e));
}

t_crd					ysonar(t_gm *gm, t_crd o, char t, int size)
{
	t_crd				out;

	if (yis_coord((out = ysonar_quarter(gm, o, t, (t_crd){o.x + size, o.y +
		size})), &gm->map))
		return (out);
	if (yis_coord((out = ysonar_quarter(gm, o, t, (t_crd){o.x + size, o.y -
		size})), &gm->map))
		return (out);
	if (yis_coord((out = ysonar_quarter(gm, o, t, (t_crd){o.x - size, o.y -
		size})), &gm->map))
		return (out);
	if (yis_coord((out = ysonar_quarter(gm, o, t, (t_crd){o.x - size, o.y +
		size})), &gm->map))
		return (out);
	return ((t_crd){-1, -1});
}
