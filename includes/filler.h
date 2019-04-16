/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 15:44:53 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/12 13:32:38 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILLER_H
# define FILLER_H

# include "ft_printf.h"
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# ifndef BUFF_SIZE
#  define BUFF_SIZE 256
# endif
# ifndef MAX_BUFF
#  define MAX_BUFF BUFF_SIZE * 4096
# endif
# define INPUT 0
# define PLATEAU "Plateau "
# define PIECE "Piece "

typedef struct			s_string
{
	char				*s;
	ssize_t				l;
}						t_string;

typedef struct			s_find_end
{
	char				*out;
	char				*rmn;
}						t_fe;

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

typedef struct 			s_map_iterator
{
	t_crd				hp;
	t_crd				vp;
}						t_m_it;

typedef struct			s_pc_navigation
{
	t_m_it				it;
	t_crd				p;
	t_crd				m_o;
	t_crd				m_p;
}						t_pc_nav;

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
** a : anchor
*/
typedef struct			s_map
{
	int					w;
	int					h;
	t_are				**m;
	t_crd				a;
}						t_map;

typedef struct 			s_direction
{
	unsigned char		(*yrng_o)(t_map*, t_crd, t_crd*);
	void				(*yrng_i)(t_map*, t_crd, t_crd*);
}						t_dir;

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
	t_m_it				it;
}						t_ply;

/*
 * map : piece's map
 * mic : minimum coordinate
 * mac : maximum coordinate
 * hp : horizeontal position
 * vp : vertical position
 * a : anchor
 */
typedef struct			s_piece
{
	t_map				map;
	t_crd				mic;
	t_crd				mac;
	t_m_it				it;
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
char					*yread(ssize_t n, char *end);

/*
 * decode_input.c
 */
unsigned int			yatoui_limited(t_in *in);
//unsigned char			ydecode_input(t_in in, t_map *map, t_pc *pc, char o);
void					yfree_turn(t_map *map, t_pc *pc);
unsigned char			yread_turn(t_in *in, t_gm *gm);

/*
 * decode_player.c
 */
unsigned char			ydecode_player(t_in in, t_ply *me, t_ply *en);

/*
 * play.c
 */
unsigned char			yplay(t_gm *gm);

#endif
