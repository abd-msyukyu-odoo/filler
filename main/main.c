/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 16:25:26 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/05 17:51:52 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

int				yon_release(int key, void *param)
{
	param = NULL;
	if (key == ESC_KEY)
		exit(0);
	return (1);
}

void			print_map_piece(t_map map, t_pc pc)
{
	int				y, x;

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
	t_gm				gm;

	if (!(in.s = yread_input()))
		return (0);
	in.p = 0;
	if (!ydecode_player(in, &gm.me, &gm.en))
	{
		free(in.s);
		return (0);
	}
	free(in.s);
	while ((in.s = yread_input()))
	{
		in.p = 0;
		if (!ydecode_input(in, &gm.map, &gm.pc, gm.me.o))
		{
			free(in.s);
			return (0);
		}
		print_map_piece(gm.map, gm.pc);
		yfree_turn(in, &gm.map, &gm.pc);
	}
	return (0);
}
