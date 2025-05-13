/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/08 18:21:05 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/13 14:02:16 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "MLX42/MLX42.h"
#include "libft.h"
#include "color.h"
#include "minirt.h"

#define WIDTH 1024
#define HEIGHT 1024

void	clear_image(mlx_image_t *img, t_col32 col)
{
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < img->height)
	{
		x = 0;
		while (x < img->width)
		{
			mlx_put_pixel(img, x, y, col);
			++x;
		}
		++y;
	}
}

int	main(void)
{
	t_col32 bg_col = init_col32(99, 157, 219, 255);

	mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "miniRT", false);
	if (!mlx)
		return (EXIT_FAILURE);
	mlx_image_t* img_plane = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img_plane || (mlx_image_to_window(mlx, img_plane, 0, 0) < 0))
	{
		mlx_terminate(mlx);
		return (EXIT_FAILURE);
	}

	render(img_plane, bg_col);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
