/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 12:08:45 by dabeloos          #+#    #+#             */
/*   Updated: 2019/04/17 12:08:46 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

char					yc_to_upper(char c)
{
	return (((c >= 'a' && c <= 'z') ? 'A' - 'a' : 0) + c);
}

unsigned int			yatoui_limited(t_in *in)
{
	unsigned int		result;
	unsigned int		tmp;

	result = 0;
	while (in->s[in->p] >= '0' && in->s[in->p] <= '9')
	{
		tmp = result;
		result = result * 10 + in->s[in->p++] - '0';
		if (result < tmp)
			return (~((unsigned int)0));
	}
	return (result);
}

unsigned char			yvalid_for_map(char c)
{
	return (c == '.' || c == 'O' || c == 'X' || c == 'o' || c == 'x');
}

unsigned char			yvalid_for_piece(char c)
{
	return (c == '.' || c == '*');
}
