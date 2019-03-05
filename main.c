/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 16:25:26 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/05 14:48:41 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

int				yon_release(int key, void *param)
{
	if (key == ESC_KEY)
		exit(0);
	return (1);
}

void			print_map_piece(t_map map, t_pc pc)
{
	size_t				y, x;

	y = 0;
	while (y < map.h)
	{
		x = 0;
		while (x < map.w)
		{
			write(1, &(map.m[y][x].o), 1);
			x++;
		}
		y++;
		write(1, "\n", 1);
	}
	if (pc.mac.y == 0)
		return ;
	y = 0;
	while (y < pc.map.h)
	{
		x = 0;
		while (x < pc.map.w)
		{
			write(1, &(pc.map.m[y][x].o), 1);
			x++;
		}
		y++;
		write(1, "\n", 1);
	}
}

int				main(void)
{
//
//	void	*mlx_ptr;
//	void	*win_ptr;
//
//	mlx_ptr = mlx_init();
//	win_ptr = mlx_new_window(mlx_ptr, 500, 500, "mlx 42");
//	mlx_key_hook(win_ptr, yon_release, NULL);
//	mlx_loop(mlx_ptr);
	t_str				in;
	t_map				map;
	t_pc				pc;
	char				err;
	t_ply				me;
	t_ply				en;

	err = 0;
	/*
	if (!(in.s = yread_input()))
		return (0);
	in.p = 0;
	if (!ydecode_player(in, &me, &en))
		return (0);
	free(in.s);
	*/
	pc.mic = (t_crd){0, 0};
	pc.mac = (t_crd){0, 0};
	if (!(in.s = yread_input()))
		return (0);
	in.p = 0;
	if (!ydecode_input(in, &map, &pc))
		return (0);
	free(in.s);
	print_map_piece(map, pc);
	return (0);
}
