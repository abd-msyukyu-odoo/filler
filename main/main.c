/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabeloos <dabeloos@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 16:25:26 by dabeloos          #+#    #+#             */
/*   Updated: 2019/03/11 16:55:51 by dabeloos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "filler.h"
#include "libft.h"


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
			ft_printf("%c", map.m[y][x].o);
			x++;
		}
		y++;
		ft_printf("\n");
	}
	if (pc.mac.y == 0)
		return ;
	y = 0;
	while (y < pc.map.h)
	{
		x = 0;
		while (x < pc.map.w)
		{
			ft_printf("%c", pc.map.m[y][x].o);
			x++;
		}
		y++;
		ft_printf("\n");
	}
}

void			test_reading()
{
	char			*out;
	char			k = 'z';

	out = yread(5, NULL);
	ft_printf("%s\n", out);
	free(out);
	out = yread(-1, &k);
	ft_printf("%s\n", out);
	free(out);
	out = yread(7, NULL);
	ft_printf("%s\n", out);
	free(out);
}

FILE *fd;

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
	//test_reading();
	//
	
	t_in				in;
	t_gm				gm;


	fd = fopen("output_test.txt", "w");
	fclose(fd);
	if (!(in.s = yread(0, "\n")))
		return (0);
	in.p = 0;
	if (!ydecode_player(in, &gm.me, &gm.en))
	{
		free(in.s);
		return (0);
	}
	free(in.s);
	in.p = 0;
	while (yread_turn(&in, &gm))
	{
		//print_map_piece(gm.map, gm.pc);
		yplay(&gm);
		yfree_turn(&gm.map, &gm.pc);
		in.p = 0;
	}
	ft_putstr_fd("read error\n", 2);
	/*
	while ((in.s = yread_input()))
	{
		in.p = 0;
		if (!ydecode_input(in, &gm.map, &gm.pc, gm.me.o))
		{
			free(in.s);
			return (0);
		}
		print_map_piece(gm.map, gm.pc);
		//yplay(&gm);
		yfree_turn(in, &gm.map, &gm.pc);
	}
	*/
	return (0);
}
