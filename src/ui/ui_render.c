/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_render.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 15:03:00 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/29 16:36:16 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include <string.h>
#include <stdint.h>

void	draw_char(mlx_image_t *img, char c, int x, int y, uint32_t color)
{
	static const uint8_t font[128][8] = {
		[0 ... 31] = {0},
		[' '] = {0},
		['-'] = {0x00, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x00},
		['+'] = {0x00, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, 0x00},
		['.'] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18},
		['0'] = {0x3C, 0x42, 0x46, 0x4A, 0x52, 0x62, 0x42, 0x3C},
		['1'] = {0x10, 0x30, 0x50, 0x10, 0x10, 0x10, 0x10, 0x7C},
		['2'] = {0x3C, 0x42, 0x02, 0x0C, 0x30, 0x40, 0x40, 0x7E},
		['3'] = {0x3C, 0x42, 0x02, 0x1C, 0x02, 0x02, 0x42, 0x3C},
		['4'] = {0x04, 0x0C, 0x14, 0x24, 0x44, 0x7E, 0x04, 0x04},
		['5'] = {0x7E, 0x40, 0x40, 0x7C, 0x02, 0x02, 0x42, 0x3C},
		['6'] = {0x1C, 0x20, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x3C},
		['7'] = {0x7E, 0x02, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20},
		['8'] = {0x3C, 0x42, 0x42, 0x3C, 0x42, 0x42, 0x42, 0x3C},
		['9'] = {0x3C, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x04, 0x38},
		['A'] = {0x18, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42},
		['B'] = {0x7C, 0x42, 0x42, 0x7C, 0x42, 0x42, 0x42, 0x7C},
		['C'] = {0x3C, 0x42, 0x40, 0x40, 0x40, 0x40, 0x42, 0x3C},
		['D'] = {0x78, 0x44, 0x42, 0x42, 0x42, 0x42, 0x44, 0x78},
		['E'] = {0x7E, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x7E},
		['F'] = {0x7E, 0x40, 0x40, 0x7C, 0x40, 0x40, 0x40, 0x40},
		['G'] = {0x3C, 0x42, 0x40, 0x4E, 0x42, 0x42, 0x42, 0x3C},
		['H'] = {0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42},
		['I'] = {0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C},
		['J'] = {0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x48, 0x30},
		['K'] = {0x42, 0x44, 0x48, 0x70, 0x48, 0x44, 0x42, 0x42},
		['L'] = {0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7E},
		['M'] = {0x42, 0x66, 0x5A, 0x5A, 0x42, 0x42, 0x42, 0x42},
		['N'] = {0x42, 0x62, 0x52, 0x4A, 0x46, 0x42, 0x42, 0x42},
		['O'] = {0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C},
		['P'] = {0x7C, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40, 0x40},
		['Q'] = {0x3C, 0x42, 0x42, 0x42, 0x4A, 0x44, 0x3A, 0x00},
		['R'] = {0x7C, 0x42, 0x42, 0x7C, 0x48, 0x44, 0x42, 0x42},
		['S'] = {0x3C, 0x42, 0x40, 0x3C, 0x02, 0x02, 0x42, 0x3C},
		['T'] = {0xFE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
		['U'] = {0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C},
		['V'] = {0x42, 0x42, 0x42, 0x24, 0x24, 0x24, 0x18, 0x18},
		['W'] = {0x42, 0x42, 0x42, 0x42, 0x5A, 0x5A, 0x66, 0x42},
		['X'] = {0x42, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x42},
		['Y'] = {0x82, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, 0x10},
		['Z'] = {0x7E, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x7E},
	};
	unsigned char	uc;
	int				dx;
	int				dy;
	int				px;
	int				py;

	uc = (unsigned char)c;
	dy = 0;
	if (uc >= 128 || !font[uc][0])
		return ;
	while (dy < 8)
	{
		dx = 0;
		while (dx < 8)
		{
			if (font[uc][dy] & (1 << (7 - dx)))
			{
				px = x + dx;
				py = y + dy;
				if (px >= 0 && px < (int)img->width && py >= 0 && py < (int)img->height)
					mlx_put_pixel(img, px, py, color);
			}
			dx++;
		}
		dy++;
	}
}

void	draw_text(mlx_image_t *img, const char *str, t_v2f pos, uint32_t color)
{
	int		x;
	int		y;
	size_t	i;

	x = (int)pos.x;
	y = (int)pos.y;
	i = 0;
	if (y < 0 || y + 8 > (int)img->height)
		return ;
	while (str[i])
	{
		if (x + 8 > (int)img->width)
		{
			x = (int)pos.x;
			y += 10;
			if (y + 8 > (int)img->height)
				break ;
		}
		draw_char(img, str[i], x, y, color);
		x += UI_CHAR_WIDTH;
		i++;
	}
}

void	draw_rect(mlx_image_t *img, t_v2f pos, t_v2f size, uint32_t color)
{	
	const int	start_x = (int)fmax(0, pos.x);
	const int	start_y = (int)fmax(0, pos.y);
	const int	end_x = (int)fmin(img->width, pos.x + size.x);
	const int	end_y = (int)fmin(img->height, pos.y + size.y);
	int			x;
	int			y;
	
	x = start_x;
	y = start_y;
	while (y < end_y)
	{
		while (x < end_x)
		{
			mlx_put_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
}


void	draw_button(t_ui_element *button, mlx_image_t *target)
{
	t_v2f				size;
	t_ui_style			style;
	t_ui_button			*btn_data;
	char				value_str[32];
	t_ui_value_button	*value_btn;
	int					text_width;
	int					plus_x;

	size = button->size;
	style = button->style;
	btn_data = (t_ui_button *)button->data;
	draw_rect(target, init_v2f(0, 0), size, style.bg_color);
	draw_rect(target, init_v2f(0, 0), init_v2f(size.x, 1), style.border_color);
	draw_rect(target, init_v2f(0, size.y - 1), init_v2f(size.x, 1), style.border_color);
	if (btn_data && btn_data->label)
	{
		draw_text(target, btn_data->label, init_v2f(style.padding, style.padding), style.text_color);
		if (button->type == UI_VALUE_BUTTON)
		{
			value_btn = (t_ui_value_button *)button->data;
			snprintf(value_str, sizeof(value_str), "%.2f", *value_btn->value);
			text_width = strlen(value_str) * UI_CHAR_WIDTH;
			draw_text(target, value_str, 
				init_v2f(size.x - text_width - style.padding, style.padding),
				style.text_color);
			draw_rect(target, init_v2f(style.padding, size.y/2 - 5),
				init_v2f(10, 1), style.text_color);
			plus_x = size.x - style.padding - 15;
			draw_rect(target, init_v2f(plus_x, size.y/2 - 5),
				init_v2f(10, 1), style.text_color);
			draw_rect(target, init_v2f(plus_x + 4, size.y/2 - 9),
				init_v2f(1, 10), style.text_color);
		}
	}
}
void	render_ui_element(t_ui_element *element, mlx_image_t *target)
{
	t_ui_element *child;

	if (!element || !element->visible)
		return;
	if (element->image && element->instance_id >= 0)
	{
		element->image->instances[element->instance_id].x = (int)element->pos.x;
		element->image->instances[element->instance_id].y = (int)element->pos.y;
	}
	child = element->first_child;
	while (child)
	{
		render_ui_element(child, target);
		child = child->next_sibling;
	}
	layout_vertical(element, 5);
}


