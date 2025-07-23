/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:00:00 by bewong            #+#    #+#             */
/*   Updated: 2025/07/24 00:01:08 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/**
 * Draws a character on the image using a simple bitmap font.
 */
static void draw_char(mlx_image_t *img, char c, int x, int y, uint32_t color)
{
	static const uint8_t font[128][8] = {
		[0 ... 31] = {0},
		[' '] = {0},
		['-'] = {0x00, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x00},
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
	int				dy;
	int				dx;
	int				px;
	int				py;
	bool			is_blank;
	int				i;

	uc = (unsigned char)c;
	is_blank = true;
	i = 0;
	while (i < 8)
	{
		if (font[uc][i] != 0) 
		{
			is_blank = false;
			break ;
		}
		i++;
	}
	if (uc >= 128 || is_blank)
		return ;
	dy = 0;
	while (dy < 8)
	{
		dx = 0;
		while (dx < 8)
		{
			if (font[uc][dy] & (1 << (7 - dx)))
			{
				px = x + dx;
				py = y + dy;
				if (px >= 0 && px < (int)img->width && 
					py >= 0 && py < (int)img->height)
					mlx_put_pixel(img, px, py, color);
			}
			dx++;
		}
		dy++;
	}
}

/**
 * Draws a string on the image at the specified position.
 * Uses a simple bitmap font for text rendering.
 */
void	rt_put_string(mlx_image_t *img, const char *str, int x, int y)
{
	int			start_x;
	int			char_width;
	int			line_height;
	uint32_t	color;

	start_x = x;
	char_width = UI_CHAR_WIDTH;
	line_height = UI_CHAR_HEIGHT;
	color = UI_TEXT_COLOR;
	while (*str)
	{
		if (*str == '\n')
		{
			y += line_height;
			x = start_x;
		}
		else if (*str == '\t')
			x += char_width * 4;
		else if (*str == '\r')
			x = start_x;
		else
		{
			draw_char(img, *str, x, y, color);
			x += char_width;
		}
		str++;
	}
}

static	void draw_rect(mlx_image_t *img, int x, int y, int width, int height, uint32_t color)
{
	int	dy;
	int	dx;

	dy = 0;
	while (dy < height)
	{
		dx = 0;
		while (dx < width)
		{
			if (y + dy >= 0 && y + dy < (int)img->height &&
				x + dx >= 0 && x + dx < (int)img->width)
				mlx_put_pixel(img, x + dx, y + dy, color);
			dx++;
		}
		dy++;
	}
}

/**
 * Draws a button on the image at the specified position.
 */
void	draw_button(mlx_image_t *img, t_button *button, int y_offset)
{
	char		value_str[32];
	uint32_t	color;
	int			x;
	int			y;

	rt_put_string(img, button->label, button->x, button->y + y_offset - 15);
	snprintf(value_str, sizeof(value_str), "%.2f", *button->value);
	if (button->is_hovered)
		color = UI_BUTTON_HOVER_COLOR;
	else
		color = UI_BUTTON_COLOR;
	draw_rect(img, button->x, button->y + y_offset,
		button->width, button->height, color);
	if (button->is_hovered)
		color = UI_BUTTON_BORDER_HOVER_COLOR;
	else
		color = UI_BUTTON_BORDER_COLOR;
	x = 0;
	while (x < button->width)
	{
		mlx_put_pixel(img, button->x + x, button->y + y_offset, color);
		mlx_put_pixel(img, button->x + x, 
			button->y + y_offset + button->height - 1, color);
		x++;
	}
	y = 0;
	while (y < button->height)
	{
		mlx_put_pixel(img, button->x, button->y + y_offset + y, color);
		mlx_put_pixel(img, button->x + button->width - 1, 
			button->y + y_offset + y, color);
		y++;
	}
	rt_put_string(img, value_str, 
		button->x + (button->width - (int)ft_strlen(value_str) * UI_CHAR_WIDTH) / 2, 
		button->y + y_offset + (button->height / 2) - 4);
	x = -5;
	while (x <= 5)
	{
		mlx_put_pixel(img, button->x + 15 + x, 
			button->y + y_offset + (button->height / 2), UI_TEXT_COLOR);
		x++;
	}
	x = -5;
	while (x <= 5)
	{
		mlx_put_pixel(img, button->x + button->width - 15 + x, 
			button->y + y_offset + (button->height / 2), UI_TEXT_COLOR);
		if (x >= -1 && x <= 1)
		{
			y = -5;
			while (y <= 5)
			{
				mlx_put_pixel(img, button->x + button->width - 15, 
					button->y + y_offset + (button->height / 2) + y, UI_TEXT_COLOR);
				y++;
			}
		}
		x++;
	}
}

void	draw_section_header(mlx_image_t *img, const char *title, int x, int y)
{
	int	title_len;
	int	title_x;

	draw_rect(img, x, y, 220, UI_SECTION_HEADER_HEIGHT, UI_SECTION_HEADER_COLOR);
	title_len = ft_strlen(title);
	title_x = x + (200 - (title_len * UI_CHAR_WIDTH)) / 2;
	if (title_x < x + 10)
		title_x = x + 10;
	rt_put_string(img, title, title_x, y + 8);
}

void	draw_submit_button(mlx_image_t *img, const char *title, int x, int y)
{
	int	title_len;
	int	title_x;

	draw_rect(img, x, y, 220, UI_SECTION_HEADER_HEIGHT, UI_SUBMIT_BUTTON_COLOR);
	title_len = ft_strlen(title);
	title_x = x + (200 - (title_len * UI_CHAR_WIDTH)) / 2;
	if (title_x < x + 10)
		title_x = x + 10;
	rt_put_string(img, title, title_x, y + 8);
}

// Check if a point is inside a button
bool	is_point_in_button(t_button *button, int x, int y, int y_offset)
{
	return (x >= button->x && x <= button->x + button->width &&
			y >= button->y + y_offset && y <= button->y + y_offset + button->height);
}

// Update button value based on click position
void update_button_value(t_button *button, int mouse_x, int y_offset)
{
	(void)y_offset;

	int	button_center_x;
	button_center_x = button->x + (button->width / 2);
	if (mouse_x < button_center_x)
		*button->value = fmax(button->min, *button->value - button->step);
	else
		*button->value = fmin(button->max, *button->value + button->step);
	printf("%s updated to %f\n", button->label, *button->value);
}

// Helper function to initialize a button with common settings
static t_button init_button(float *value, const char *label, float min, float max, float step)
{
	t_button button = {
		.x = UI_BUTTON_SPACING,
		.y = 0,
		.width = UI_BUTTON_WIDTH,
		.height = UI_BUTTON_HEIGHT,
		.min = min,
		.max = max,
		.step = step,
		.is_hovered = false
	};
	
	// Safely copy the label string
	if (label)
		strncpy(button.label, label, sizeof(button.label) - 1);
	button.label[sizeof(button.label) - 1] = '\0';
	button.value = value;
	return button;
}

// Initialize environment buttons with hardcoded values TODO: update the correct one
void init_ambient_buttons(t_ui *ui, t_scene *scene)
{
	ui->am_r_button = init_button(
		&scene->camera.bg_color.x, "COL R", 0.0f, 255.0f, 5.0f);
	ui->am_g_button = init_button(
		&scene->camera.bg_color.y, "COL G", 0.0f, 255.0f, 5.0f);
	ui->am_b_button = init_button(
		&scene->camera.bg_color.z, "COL B", 0.0f, 255.0f, 5.0f);
	ui->am_inten_button = init_button(
		&scene->camera.bg_color.z, "INTENSITY", 0.0f, 1.0f, 0.1f);

	// ((t_light *)scene->lights.items[0])->type == LIGHT_AMBIENT;
	
}

// Initialize camera buttons with values from the parsed scene
void	init_camera_buttons(t_ui *ui, t_cam *camera)
{
	ui->pos_x_button = init_button(
		&camera->t.pos.x, "POS X", -100.0f, 100.0f, 1.0f);
	ui->pos_y_button = init_button(
		&camera->t.pos.y, "POS Y", -100.0f, 100.0f, 1.0f);
	ui->pos_z_button = init_button(
		&camera->t.pos.z, "POS Z", -100.0f, 100.0f, 1.0f);
	ui->dir_x_button = init_button(
		&camera->t.dir.x, "DIR X", -1.0f, 1.0f, 0.1f);
	ui->dir_y_button = init_button(
		&camera->t.dir.y, "DIR Y", -1.0f, 1.0f, 0.1f);
	ui->dir_z_button = init_button(
		&camera->t.dir.z, "DIR Z", -1.0f, 1.0f, 0.1f);
	ui->cam_fov_button = init_button(
		&camera->fov, "FOV", 0.0f, 180.0f, 5.0f);
	printf("POS X value ptr: %p = %f\n", (void*)&camera->t.pos.x, camera->t.pos.x);
	printf("DIR X value ptr: %p = %f\n", (void*)&camera->t.dir.x, camera->t.dir.x);

	printf("POS X button.value: %p\n", (void*)ui->pos_x_button.value);
	printf("DIR X button.value: %p\n", (void*)ui->dir_x_button.value);
}

// Initialize point light controls
void	init_point_light_buttons(t_ui *ui, t_scene *scene)
{
	t_light	*point_light;
	t_light	*light;
	int		i;

	point_light = NULL;
	i = 0;
	while (i < (int)scene->lights.size) 
	{
		light = vector_get(&scene->lights, i);
		if (light && light->type == LIGHT_POINT)
		{
			point_light = light;
			break ;
		}
		i++;
	}
	if (point_light)
	{
		ui->light_x_button = init_button(
			&point_light->pos.x, "LIGHT X", -10.0f, 10.0f, 0.5f);
		ui->light_y_button = init_button(
			&point_light->pos.y, "LIGHT Y", -10.0f, 10.0f, 0.5f);
		ui->light_z_button = init_button(
			&point_light->pos.z, "LIGHT Z", -10.0f, 10.0f, 0.5f);
		ui->light_intensity_button = init_button(
			&point_light->intensity, "INTENSITY", 0.0f, 1.0f, 0.1f);
	}
}


// Initialize all UI elements with values from the parsed scene
void ui_init(t_ui *ui, t_scene *scene)
{
	init_ambient_buttons(ui, scene);
	init_camera_buttons(ui, &scene->camera);
	init_point_light_buttons(ui, scene);
	ui->show_ui = true;
	ui->show_ambient = true;
	ui->show_camera = true;
	ui->show_lights = true;

}

/**
 * Draws the main UI overlay
 */
void draw_ui(t_game *game)
{
	uint32_t	y;
	uint32_t	x;
	int		y_offset;

	if (!game || !game->ui.ui_layer || !game->ui.ui_layer->pixels)
		return ;

	if (!game->ui.show_ui)
	{
		ft_bzero(game->ui.ui_layer->pixels, 
			game->ui.ui_layer->width * game->ui.ui_layer->height * 4);
		return ;
	}
	y = 0;
	while (y < game->ui.ui_layer->height)
	{
		x = 0;
		while (x < game->ui.ui_layer->width)
		{
			uint8_t *pixel = &game->ui.ui_layer->pixels[(y * game->ui.ui_layer->width + x) * 4];
			if (x < UI_PANEL_WIDTH)
			{
				pixel[0] = (UI_PANEL_BG_COLOR >> 24) & 0xFF;
				pixel[1] = (UI_PANEL_BG_COLOR >> 16) & 0xFF;
				pixel[2] = (UI_PANEL_BG_COLOR >> 8) & 0xFF;
				pixel[3] = UI_PANEL_BG_COLOR & 0xFF;
			}
			else
				*(uint32_t *)pixel = 0x00000000;
			x++;
		}
		y++;
	}

	y_offset = UI_BUTTON_SPACING;

	// Draw Environment Section
	draw_section_header(game->ui.ui_layer, "AMBIENT LIGHT", UI_BUTTON_SPACING, y_offset);
	y_offset += UI_SECTION_HEADER_HEIGHT + (UI_BUTTON_SPACING * 1.5);

	if (game->ui.show_ambient)
	{
		game->ui.am_r_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.am_r_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.am_g_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.am_g_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.am_b_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.am_b_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.am_inten_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.am_inten_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.3);
	}
	// Draw Camera Section
	draw_section_header(game->ui.ui_layer, "CAMERA", UI_BUTTON_SPACING, y_offset);
	y_offset += UI_SECTION_HEADER_HEIGHT + (UI_BUTTON_SPACING * 1.5);

	if (game->ui.show_camera)
	{
		game->ui.pos_x_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.pos_x_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.pos_y_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.pos_y_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.pos_z_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.pos_z_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.dir_x_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.dir_x_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.dir_y_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.dir_y_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.dir_z_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.dir_z_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.cam_fov_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.cam_fov_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.3);
	}
	// Draw Light Section
	draw_section_header(game->ui.ui_layer, "POINT LIGHT", UI_BUTTON_SPACING, y_offset);
	y_offset += UI_SECTION_HEADER_HEIGHT + (UI_BUTTON_SPACING * 1.5);

	if (game->ui.show_lights)
	{
		game->ui.light_x_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.light_x_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.light_y_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.light_y_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.light_z_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.light_z_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.2);
		game->ui.light_intensity_button.y = y_offset;
		draw_button(game->ui.ui_layer, &game->ui.light_intensity_button, 0);
		y_offset += UI_BUTTON_HEIGHT + (UI_BUTTON_SPACING * 1.3);
	}

	draw_submit_button(game->ui.ui_layer, "SUBMIT", UI_BUTTON_SPACING, y_offset);
	y_offset += UI_SECTION_HEADER_HEIGHT + (UI_BUTTON_SPACING * 1.5);
}
