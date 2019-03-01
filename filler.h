/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:44:53 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/01 18:59:09 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLER_H
# define FILLER_H

# include "keycode.h"
# include "minilibx_macos/mlx.h"
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# define BUFF_SIZE 50
# define STD_OUT 1
# define MAP struct s_map
# define RNG struct s_range
# define ARE struct s_are

#include <stdio.h>

/*
** s : start
** e : end
*/
typedef struct			s_range
{
	int					s;
	int					e;
}						t_range;

/*
** v : vertical
** h : horizontal
** o : owner
*/
typedef struct			s_are
{
	RNG					v;
	RNG					h;
	char				o;
}						t_are;

/*
** w : width
** h : height
** m : map
*/
typedef struct			s_map
{
	int					w;
	int					h;
	ARE					**m;
}						t_map;

/*
** read.c
*/
char					*yread_input();

/*
 * decode_input.c
 */
unsigned char			decode_input(char *in, MAP *map);

#endif
