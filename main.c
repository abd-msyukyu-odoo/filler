/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 16:25:26 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/05 13:56:10 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"

int				yon_release(int key, void *param)
{
	if (key == ESC_KEY)
		exit(0);
	return (1);
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
	in.s = yread_input();
	in.p = 0;
	if (!ydecode_player(in, &me, &en))
		return (0);
	free(in.s);
	pc.mic = (t_crd){0, 0};
	pc.mac = (t_crd){0, 0};
	in.s = yread_input();
	in.p = 0;
	if (!ydecode_input(in, &map, &pc))
		return (0);
	free(in.s);
	return (0);
}
