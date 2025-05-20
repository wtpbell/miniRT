/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/19 15:00:42 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/19 18:13:55 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../../lib/mlx42/src/font/font.h"
#include "MLX42/MLX42_Int.h"
#include "libft.h"

/**
 * Does the actual copying of pixels form the atlas buffer to the
 * image buffer.
 *
 * Skips any non-printable characters.
 *
 * @param image The image to draw on.
 * @param texture The font_atlas.
 * @param texoffset The character texture X offset.
 * @param imgoffset The image X offset.
 */
static void rt_draw_char(mlx_image_t* image, int32_t texoffset, int32_t imgoffset, int32_t offset)
{
	char*		pixelx;
	uint8_t*	pixeli;

	if (texoffset < 0)
		return;
	for (uint32_t y = 0; y < FONT_HEIGHT; y++)
	{
		pixelx = &font_atlas.pixels[(y * font_atlas.width + texoffset) * BPP];
		pixeli = image->pixels + ((offset + y * image->width + imgoffset) * BPP);
		ft_memcpy(pixeli, pixelx, FONT_WIDTH * BPP);
	}
}

/* Modified code from MLX42 library */
mlx_image_t* rt_put_string(mlx_image_t *strimage, const char* str, int32_t x, int32_t y)
{
	const size_t	len = strlen(str);
	int32_t			imgoffset;

	(void)x;
	(void)y;
	if (len > MLX_MAX_STRING)
		return ((void*)mlx_error(MLX_STRTOOBIG));
	imgoffset = 0;
	for (size_t i = 0; i < len; i++, imgoffset += FONT_WIDTH)
	{
		rt_draw_char(strimage, mlx_get_texoffset(str[i]), imgoffset, y * strimage->width + x);
	}

	return (strimage);
}
