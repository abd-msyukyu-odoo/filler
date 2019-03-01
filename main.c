#include "minilibx_macos/mlx.h"
#include "keycode.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#define BUFF_SIZE 50
#define STD_OUT 1

int				yon_release(int key, void *param)
{
	if (key == ESC_KEY)
		exit(0);
	return (1);
}

unsigned char	ystr_realloc(char **in, size_t old, size_t new)
{
	char		*out;

	if (old == 0 || new < old || (*in == NULL && old != 0))
		return (0);
	out = (char*)malloc(new + 1);
	if (!out)
		return (0);
	while (old-- > 0)
		out[old] = *in[old];
	out[new] = '\0';
	free(*in);
	*in = out;
	return (1);
}

ssize_t			ystr_join(char **des, size_t ld, char *src, size_t ls)
{
	if (!ystr_realloc(des, ld, ld + ls))
		return (-1);
	while (ls-- > 0)
		*des[ld + ls] = src[ls];
	return (ld + ls);
}

char			*yread_input()
{
	char			buff[BUFF_SIZE];
	ssize_t			nread;
	char			*out;
	ssize_t			len;

	len = 0;
	out = NULL;
	while ((nread = read(STD_OUT, buff, BUFF_SIZE)))
	{
		if (nread == -1 || -1 == (len = ystr_join(&out, len, buff, BUFF_SIZE)))
			return (NULL);
	}
}

int				main(void)
{
	void	*mlx_ptr;
	void	*win_ptr;

	mlx_ptr = mlx_init();
	win_ptr = mlx_new_window(mlx_ptr, 500, 500, "mlx 42");
	mlx_key_hook(win_ptr, yon_release, NULL);
	mlx_loop(mlx_ptr);
	return (0);
}


