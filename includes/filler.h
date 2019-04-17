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
# include "yreader.h"
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
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
** utils.c
*/
char					yc_to_upper(char c);
unsigned int			yatoui_limited(t_in *in);
unsigned char			yvalid_for_map(char c);
unsigned char			yvalid_for_piece(char c);

/*
** freer.c
*/
void					ydereference_rng(t_map *map, int x, int y, t_rng *rng);
void					yfree_ares(t_map *map, int x, int y);
void					yfree_map(t_map *map);
void					yfree_turn(t_map *map, t_pc *pc);
void					yreset_in(t_in *in);

/*
** are_support.c
*/
void					yappend_range(t_rng **dst, t_rng *src);
unsigned char			ymalloc_range(t_rng **rng, int x, int y);

/*
** are_creator.c
*/
unsigned char			yadd_are(char o, int x, int y, t_map *map);

/*
** map_decoder.c
*/
unsigned char			ydecode_map(t_in *in, t_map *map);

/*
** map_creator.c
*/
unsigned char			ymalloc_map(t_map *map);

/*
** piece_decoder.c
*/
unsigned char			ydecode_pc(t_in *in, t_pc *pc, char o);

/*
** piece_cropper.c
*/
unsigned char			ydecode_crop(t_in *in, t_pc *pc);

/*
** dimensions_decoder.c
*/
unsigned char			ydecode_size(t_in *in, int *w, int *h, char *ref);
ssize_t					ymap_read_size(t_map *map);

/*
** turn_decoder.c
*/
unsigned char			yread_turn(t_in *in, t_gm *gm);

/*
** player_decoder.c
*/
unsigned char			ydecode_player(t_in in, t_ply *me, t_ply *en);

/*
 * play.c
 */
unsigned char			yplay(t_gm *gm);

#endif
