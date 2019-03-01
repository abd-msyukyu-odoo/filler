/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 18:26:59 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/01 18:33:37 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

static unsigned int		ft_atoui_limited(const char *str, size_t s, size_t e)
{
	unsigned int		result;
	size_t				i;
	unsigned int		tmp;

	result = 0;
	i = s;
	while (i < e && str[i] >= '0' && str[i] <= '9')
	{
		tmp = result;
		result = result * 10 + str[i++] - '0';
		if (result < tmp)
			return (~((unsigned int)0));
	}
	return (result);
}

MAP						*decode_input(char *in)
{
	
}
