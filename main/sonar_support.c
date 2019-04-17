/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sonar_support.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 13:59:59 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 14:00:00 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

void					yidentify_quarter(t_crd o, t_crd d, t_crd *s, t_crd *e)
{
	if (o.x < d.x)
	{
		*s = (t_crd){o.x, d.y};
		*e = (t_crd){d.x, o.y};
	}
	else
	{
		*s = (t_crd){d.x, o.y};
		*e = (t_crd){o.x, d.y};
	}
}

unsigned char			yfit_backslash(t_gm *gm, t_crd *s, t_crd *e)
{
	if (s->x < 0)
		*s = (t_crd){0, s->y - s->x};
	if (s->y < 0)
		*s = (t_crd){s->x - s->y, 0};
	if (!yis_coord(*s, &gm->map))
		return (0);
	if (e->x >= gm->map.w)
		*e = (t_crd){gm->map.w - 1, e->y - (e->x - gm->map.w + 1)};
	if (e->y >= gm->map.h)
		*e = (t_crd){e->x - (e->y - gm->map.h + 1), gm->map.h - 1};
	if (!yis_coord(*e, &gm->map))
		return (0);
	return (1);
}

unsigned char			yfit_slash(t_gm *gm, t_crd *s, t_crd *e)
{
	if (s->x < 0)
		*s = (t_crd){0, s->y + s->x};
	if (s->y >= gm->map.h)
		*s = (t_crd){s->x + (s->y - gm->map.h + 1), gm->map.h - 1};
	if (!yis_coord(*s, &gm->map))
		return (0);
	if (e->x >= gm->map.w)
		*e = (t_crd){gm->map.w - 1, e->y + (e->x - gm->map.w + 1)};
	if (e->y < 0)
		*e = (t_crd){e->x + e->y, 0};
	if (!yis_coord(*e, &gm->map))
		return (0);
	return (1);
}
