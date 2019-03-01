#include "mlx.h"
#include "keycode.h"
#include <stdio.h>
#include <stdlib.h>

int on_release(int key, void *param)
{
	if (key == ESC_KEY)
		exit(0);
	return (1);
}

int main()
{
	void	*mlx_ptr;
	void	*win_ptr;

	mlx_ptr = mlx_init();
	win_ptr = mlx_new_window(mlx_ptr, 500, 500, "mlx 42");
	mlx_key_hook(win_ptr, on_release, NULL);
	mlx_loop(mlx_ptr);
}
