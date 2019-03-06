/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:44:53 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/06 15:46:20 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLER_H
# define FILLER_H

# include "keycode.h"
# include "mlx.h"
# include "ft_printf.h"
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# define BUFF_SIZE 50
# define STD_OUT 1
# define PLATEAU "Plateau "
# define PIECE "Piece "

typedef struct			s_coord
{
	int					x;
	int					y;
}						t_crd;

/*
** s : start
** d : distance
*/
typedef struct			s_range
{
	t_crd				s;
	int					d;
}						t_rng;

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
typedef struct			s_input
{
	char				*s;
	size_t				p;
}						t_in;

/*
 * o : owner
 * hp : horizontal position
 * vp : vertical position
 */
typedef struct			s_player
{
	char				o;
	t_crd				hp;
	t_crd				vp;
}						t_ply;

/*
 * map : piece's map
 * mic : minimum coordinate
 * mac : maximum coordinate
 * hp : horizeontal position
 * vp : vertical position
 */
typedef struct			s_piece
{
	t_map				map;
	t_crd				mic;
	t_crd				mac;
	t_crd				hp;
	t_crd				vp;
}						t_pc;

/*
 */
typedef struct			s_game_master
{
	t_map				map;
	t_pc				pc;
	t_ply				me;
	t_ply				en;
}						t_gm;

/*
** read.c
*/
char					*yread_input();

/*
 * decode_input.c
 */
unsigned char			ydecode_input(t_in in, t_map *map, t_pc *pc, char o);
void					yfree_turn(t_in in, t_map *map, t_pc *pc);

/*
 * decode_player.c
 */
unsigned char			ydecode_player(t_in in, t_ply *me, t_ply *en);

/*
 * play.c
 */
void					yplay(t_gm *gm);

#endif
