/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:44:53 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/05 15:58:44 by dabeloos         ###   ########.fr       */
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
# define PLATEAU "Plateau "
# define PIECE "Piece "

#include <stdio.h>

/*
** s : start
** e : end
*/
typedef struct			s_range
{
	int					s;
	int					e;
}						t_rng;

typedef struct			s_coord
{
	int					x;
	int					y;
}						t_crd;

/*
** v : vertical
** h : horizontal
** b : backslash
** s : slash
** o : owner
*/
typedef struct			s_are
{
	t_rng				*v;
	t_rng				*h;
	t_rng				*b;
	t_rng				*s;
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
	t_are				**m;
}						t_map;

/*
 * s : string
 * p : position
 */
typedef struct			s_str
{
	char				*s;
	size_t				p;
}						t_str;

/*
 * o : owner
 */
typedef struct			s_ply
{
	char				o;
}						t_ply;

/*
 * map : piece's map
 * mic : minimum coordinate
 * mac : maximum coordinate
 */
typedef struct			s_pc
{
	t_map				map;
	t_crd				mic;
	t_crd				mac;
}						t_pc;

/*
** read.c
*/
char					*yread_input();

/*
 * decode_input.c
 */
unsigned char			ydecode_input(t_str in, t_map *map, t_pc *pc, char o);
void					yfree_turn(t_map *map, t_pc *pc);

/*
 * decode_player.c
 */
unsigned char			ydecode_player(t_str in, t_ply *me, t_ply *en);

#endif
