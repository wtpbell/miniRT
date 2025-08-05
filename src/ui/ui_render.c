/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_render.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/27 15:03:00 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/05 10:20:28 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

void	draw_char(mlx_image_t *canvas, char c, int x, int y, uint32_t color)
{
	// Font array with decimal indices for all printable ASCII characters
	static const uint8_t font[256][8] = {
		[0 ... 31] = {0},
		[' '] = {0},
		['.'] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18},
		['-'] = {0x00, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x00},  // Horizontal line
		['+'] = {0x00, 0x18, 0x18, 0x7E, 0x7E, 0x18, 0x18, 0x00},  // Plus sign with thicker lines
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
	if (uc < 32 && uc != ' ')
		return;
	if (uc >= 128 || (uc > 0 && ft_memcmp(font[uc], (uint8_t[8]){0}, 8) == 0 && uc != ' '))
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
				if (px >= 0 && py >= 0 && px < (int)canvas->width && py < (int)canvas->height)
				{
					if ((color >> 24) != 0x00)
					{
						uint32_t *pixel = (uint32_t *)canvas->pixels + py * canvas->width + px;
						*pixel = color;
					}
				}
			}
			dx++;
		}
		dy++;
	}
}

void	draw_text(mlx_image_t *canvas, const char *str, t_v2f pos, uint32_t color)
{
	int		x;
	int		y;
	size_t	i;

	x = (int)pos.x;
	y = (int)pos.y;
	i = 0;
	
	if (y < 0 || y + 8 > (int)canvas->height)
		return ;
	while (str[i])
	{
		if (x + 8 > (int)canvas->width)
		{
			x = (int)pos.x;
			y += 10;
			if (y + 8 > (int)canvas->height)
				break ;
		}
		draw_char(canvas, str[i], x, y, color);
		x += UI_CHAR_WIDTH;
		i++;
	}
}

void	draw_rect_border(mlx_image_t *canvas, t_v2f pos, t_v2f size, uint32_t color)
{
	int			x;
	int			y;
	int			end_x;
	int			end_y;
	uint32_t	*pixel;

	x = (int)pos.x;
	y = (int)pos.y;
	end_x = x + (int)size.x;
	end_y = y + (int)size.y;
	while (y < end_y)
	{
		x = (int)pos.x;
		while (x < end_x)
		{
			if ((x == (int)pos.x || x == end_x - 1 || y == (int)pos.y || y == end_y - 1) &&
				x >= 0 && y >= 0 && x < (int)canvas->width && y < (int)canvas->height)
			{
				pixel = (uint32_t *)canvas->pixels + y * canvas->width + x;
				*pixel = color;
			}
			x++;
		}
		y++;
	}
}

// Helper function for alpha blending
static uint32_t blend_colors(uint32_t bg, uint32_t fg)
{
	float alpha = ((fg >> 24) & 0xFF) / 255.0f;
	float inv_alpha = 1.0f - alpha;

	uint8_t r = (uint8_t)(((bg >> 16) & 0xFF) * inv_alpha + ((fg >> 16) & 0xFF) * alpha);
	uint8_t g = (uint8_t)(((bg >> 8) & 0xFF) * inv_alpha + ((fg >> 8) & 0xFF) * alpha);
	uint8_t b = (uint8_t)((bg & 0xFF) * inv_alpha + (fg & 0xFF) * alpha);
	uint8_t a = (uint8_t)(0xFF);

	return (a << 24) | (r << 16) | (g << 8) | b;
}

void	draw_rect(mlx_image_t *canvas, t_v2f pos, t_v2f size, uint32_t color)
{
	int x, y;
	int start_x, start_y, end_x, end_y;

	start_x = (int)pos.x;
	start_y = (int)pos.y;
	end_x = start_x + (int)size.x;
	end_y = start_y + (int)size.y;
	if (start_x < 0) start_x = 0;
	if (start_y < 0) start_y = 0;
	if (end_x > (int)canvas->width) end_x = canvas->width;
	if (end_y > (int)canvas->height) end_y = canvas->height;
	if (start_x >= (int)canvas->width || start_y >= (int)canvas->height ||
		end_x <= 0 || end_y <= 0)
		return;
	if ((color >> 24) == 0x00)
		return;
	if ((color >> 24) == 0xFF)
	{
		y = start_y;
		while (y < end_y)
		{
			x = start_x;
			while (x < end_x)
			{
				uint32_t *pixel = (uint32_t *)canvas->pixels + y * canvas->width + x;
				*pixel = color;
				x++;
			}
			y++;
		}
	}
	else
	{
		y = start_y;
		while (y < end_y)
		{
			x = start_x;
			while (x < end_x)
			{
				uint32_t *pixel = (uint32_t *)canvas->pixels + y * canvas->width + x;
				*pixel = blend_colors(*pixel, color);
				x++;
			}
			y++;
		}
	}
}

void	draw_button(t_ui_element *button, t_ui_context *ctx)
{
	t_ui_button	*btn_data;
	t_v2f		text_pos;
	int			text_width;

	draw_rect(ctx->canvas, button->abs_pos, button->size, UI_BUTTON_COLOR);
	draw_rect_border(ctx->canvas, button->abs_pos, button->size, UI_BUTTON_BORDER_COLOR);
	btn_data = (t_ui_button *)button->data;
	if (btn_data && btn_data->label)
	{
		text_width = ft_strlen(btn_data->label) * UI_CHAR_WIDTH;
		text_pos.x = button->abs_pos.x + (button->size.x - text_width) / 2;
		text_pos.y = button->abs_pos.y + (button->size.y - UI_CHAR_HEIGHT) / 1.5;
		draw_text(ctx->canvas, btn_data->label, text_pos, UI_TEXT_COLOR);
	}
}

void	render_ui_element(t_ui_element *element, t_ui_context *ctx)
{
	t_ui_element		*child;
	t_ui_label			*label;
	// t_ui_value_button	*vbutton;

	if (!element->visible)
		return ;
	element->abs_pos = element->pos;
	if (element->parent)
	{
		element->abs_pos.x += element->parent->abs_pos.x + element->parent->style.padding;
		element->abs_pos.y += element->parent->abs_pos.y + element->parent->style.padding;
	}
	if (element->type == UI_PANEL || element->type == UI_HEADER)
	{
		draw_rect(ctx->canvas, element->abs_pos, element->size, element->style.bg_color);
		draw_rect_border(ctx->canvas, element->abs_pos, element->size, element->style.border_color);
	}
	else if (element->type == UI_BUTTON)
		draw_button(element, ctx);
	else if (element->type == UI_LABEL)
	{
		label = (t_ui_label *)element->data;
		draw_text(ctx->canvas, label->text, element->abs_pos, element->style.text_color);
	}
	// else if (element->type == UI_VALUE_BUTTON)
	// {
	// 	vbutton = (t_ui_value_button *)element->data;
	// 	draw_text(ctx->canvas, vbutton->label, element->abs_pos, element->style.text_color);
	// }
	child = element->first_child;
	while (child)
	{
		render_ui_element(child, ctx);
		child = child->next_sibling;
	}
}
