/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   new_ui.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/23 14:48:38 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/24 14:46:41 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include <stdlib.h>
#include <stdio.h>

// UI_PANEL (root)
// ├── UI_HEADER "Ambient" (collapsible)
// │   ├── UI_BUTTON "Intensity" (-    value    +)
// │   ├── UI_BUTTON "R"(-    value    +)
// │   ├── UI_BUTTON "G"(-    value    +)
// │   └── UI_BUTTON "B" (-    value    +)
// │
// ├── UI_HEADER "Camera" (collapsible) 
// │   ├── UI_BUTTON "Pos X"(-    value    +)
// │   ├── UI_BUTTON "Pos Y" (-    value    +)
// │   ├── UI_BUTTON "Pos Z" (-    value    +)
// │   ├── UI_BUTTON "Dir X" (-    value    +)
// │   ├── UI_BUTTON "Dir Y" (-    value    +)
// │   ├── UI_BUTTON "Dir Z" (-    value    +)
// │   └── UI_BUTTON "FOV"(-    value    +)
// │
// └── UI_HEADER "Lights" (collapsible)
// │   ├── UI_BUTTON "Light X"(-    value    +)
// │   ├── UI_BUTTON "Light Y" (-    value    +)
// │   ├── UI_BUTTON "Light Z"(-    value    +)
// │   ├── UI_BUTTON "Intensity"(-    value    +)
// │
// └── BOTTON "Submit" (action)

t_ui_element *ui_element_create(t_ui_type type, t_v2f pos, t_v2f size)
{
	t_ui_element	*element;

	element = malloc(sizeof(t_ui_element));
	if (!element)
		return (NULL);
	element->style.bg_color = 0x111111FF;
	element->style.fg_color = 0xFFFFFFFF;
	element->style.text_color = 0xFFFFFFFF;
	element->style.border_color = 0xFFFFFFFF;
	element->style.padding = 5;
	element->style.visible = true;

	element->type = type;
	element->pos = pos;
	element->size = size;
	element->parent = NULL;
	element->first_child = NULL;
	element->next_sibling = NULL;
	element->data = NULL;
	element->action = NULL;


	if (type == UI_PANEL)
	{
		element->style.bg_color = 0x111111FF;
	}
	else if (type == UI_SECTION)
	{
		element->style.bg_color = 0x222222FF;
	}
	else if (type == UI_HEADER)
	{
		element->style.bg_color = 0x333333FF;
		element->style.text_color = 0xFFFFFFFF;
	}
	else if (type == UI_BUTTON)
	{
		element->style.bg_color = 0x444444FF;
		element->style.text_color = 0xFFFFFFFF;
	}
	else if (type == UI_LABEL)
	{
		element->style.bg_color = 0x00000000; 
		element->style.text_color = 0xFFFFFFFF;
	}

	return (element);
}

void	ui_add_child(t_ui_element *parent, t_ui_element *child)
{
	t_ui_element	*current;

	if (!parent->first_child)
		parent->first_child = child;
	else
	{
		current = parent->first_child;
		while(current->next_sibling)
			current = current->next_sibling;
		current->next_sibling = child;
	}
	child->parent = parent;
}

static void ui_element_set_style(t_ui_element *element, t_ui_style style)
{
	if (!element)
		return;
	element->style = style;
}

t_ui_element *create_panel(t_v2f pos, t_v2f size)
{
	t_ui_element *panel;

	panel = ui_element_create(UI_PANEL, pos, size);
	if (!panel)
		return (NULL);
	ui_element_set_style(panel, (t_ui_style){
		.bg_color = UI_PANEL_BG_COLOR,
		.fg_color = 0x00000000,
		.text_color = UI_TEXT_COLOR,
		.border_color = 0x00000000,
		.padding = 10,
		.visible = true
	});
	return (panel);
}

t_ui_element *create_section(const char *title, t_v2f pos, t_v2f size)
{
	t_ui_element *section;
	section = ui_element_create(UI_SECTION, pos, size);
	if (!section)
		return (NULL);

	ui_element_set_style(section, (t_ui_style){
		.bg_color = UI_SECTION_COLOR,
		.fg_color = 0x00000000,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = 5,
		.visible = true
	});

	if (title)
	{
		t_ui_element *header = create_header(title, init_v2f(0, 0),
			init_v2f(size.x, UI_SECTION_HEADER_HEIGHT));
		if (header)
		{
			ui_add_child(section, header);
		}
	}

	return (section);
}

t_ui_element *create_header(const char *title, t_v2f pos, t_v2f size)
{
	t_ui_element *header;
	t_ui_label *label_data;

	header = ui_element_create(UI_HEADER, pos, size);
	if (!header)
		return (NULL);

	ui_element_set_style(header, (t_ui_style){
		.bg_color = UI_HEADER_COLOR,
		.fg_color = 0x00000000,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = 5,
		.visible = true
	});

	label_data = malloc(sizeof(t_ui_label));
	if (!label_data)
	{
		free(header);
		return (NULL);
	}
	
	label_data->text = ft_strdup(title);
	if (!label_data->text)
	{
		free(label_data);
		free(header);
		return (NULL);
	}
	
	label_data->color = UI_TEXT_COLOR;
	header->data = label_data;

	return (header);
}

t_ui_element *create_label(const char *title, t_v2f pos, uint32_t color)
{
	t_ui_element *label;
	t_ui_label *label_data;

	label = ui_element_create(UI_LABEL, pos, init_v2f(UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT));
	if (!label)
		return (NULL);
	ui_element_set_style(label, (t_ui_style){
		.bg_color = 0x00000000,
		.fg_color = 0x00000000,
		.text_color = color,
		.border_color = 0x00000000,
		.padding = 2,
		.visible = true
	});
	label_data = malloc(sizeof(t_ui_label));
	if (!label_data)
	{
		free(label);
		return (NULL);
	}
	label_data->text = ft_strdup(title);
	if (!label_data->text)
	{
		free(label_data);
		free(label);
		return (NULL);
	}
	label_data->color = color;
	label->data = label_data;
	return (label);
}

t_ui_element	*create_value_button(const char *label, float *value,
								t_v2f range, float step)
{
	t_ui_element	*button;
	t_ui_button		*btn;

	button = ui_element_create(UI_BUTTON, g_v2f_zero, init_v2f(UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT));
	if (!button)
		return (NULL);
	btn = malloc(sizeof(t_ui_button));
	if (!btn)
		return (free(button), NULL);
	btn->label_text = ft_strdup(label);
	if (!btn->label_text)
		return (free(btn), free(button), NULL);
	
	btn->value = value;
	btn->range = range;
	btn->step = step;
	btn->instance_id = btn->image->count - 1;
	*value = fmax(range.x, fmin(range.y, *value));
	button->data = btn;
	return (button);
}

static void	position_vertically(t_ui_element *element, t_v2f pos, float spacing)
{
	if (!element)
		return;
	element->pos = pos;
	if (element->next_sibling)
	{
		position_vertically(element->next_sibling, 
						init_v2f(pos.x, pos.y + element->size.y + spacing), 
						spacing);
	}
}

t_ui_element	*create_ambient_section(t_scene *scene, t_v2f pos)
{
	t_ui_element	*section;
	t_ui_element	*col_r;
	t_ui_element	*col_g;
	t_ui_element	*col_b;
	t_ui_element	*am_inten;
	t_light			*light;
	t_light			*am_light = NULL;
	int				i;

	i = 0;
	while (i < scene->lights.size)
	{
		light = (t_light *)scene->lights.items[i];
		if (light->type == LIGHT_AMBIENT)
		{
			am_light = light;
			break;
		}
		i++;
	}
	section = create_section("AMBIENT", pos, init_v2f(UI_PANEL_WIDTH, 200));
	if (!section)
	{
		// if (am_light->type == LIGHT_AMBIENT)
		// 	free(am_light);
		return (NULL);
	}
	col_r = create_value_button("COL R", &am_light->color.x, init_v2f(0, 255), 5.0f);
	col_g = create_value_button("COL G", &am_light->color.y, init_v2f(0, 255), 5.0f);
	col_b = create_value_button("COL B", &am_light->color.z, init_v2f(0, 255), 5.0f);
	am_inten = create_value_button("INTENSITY", &am_light->intensity, init_v2f(0, 1), 0.1f);

	if (!col_r || !col_g || !col_b || !am_inten)
	{
		if (col_r) free(col_r);
		if (col_g) free(col_g);
		if (col_b) free(col_b);
		if (am_inten) free(am_inten);
		// if (am_light->type == LIGHT_AMBIENT) free(am_light);
		free(section);
		return (NULL);
	}

	t_v2f current_pos = init_v2f(10, 40);
	position_vertically(col_r, current_pos, 5);
	current_pos.y += col_r->size.y + 5;
	position_vertically(col_g, current_pos, 5);
	current_pos.y += col_g->size.y + 5;
	position_vertically(col_b, current_pos, 5);
	current_pos.y += col_b->size.y + 5;
	position_vertically(am_inten, current_pos, 5);
	ui_add_child(section, col_r);
	ui_add_child(section, col_g);
	ui_add_child(section, col_b);
	ui_add_child(section, am_inten);
	// if (am_light->type == LIGHT_AMBIENT)
		section->data = am_light;

	return (section);
}

void	cleanup_ui(t_ui *ui)
{
	if (ui->ui_layer)
		mlx_delete_image(ui->mlx, ui->ui_layer);
	free(ui);
}

int game_loop(void *param)
{
	static bool first_frame = true;
	t_game *game = (t_game *)param;
	t_ui *ui;
	bool needs_scene_redraw = false;

	ui = game->ui;
	needs_scene_redraw = first_frame || ui->needs_redraw;
	first_frame = false;
	if (needs_scene_redraw) {
		printf("game_loop: Rendering scene\n");
		render_scene(game);
	}
	if (needs_scene_redraw || ui->needs_redraw) {
		printf("game_loop: Updating UI\n");
		
		if (!ui->ui_layer) {
			printf("game_loop: Error - ui->ui_layer is NULL\n");
			return (1);
		}
		if (!ui->root) {
			printf("game_loop: Error - ui->root is NULL\n");
			return (1);
		}
		ft_bzero(ui->ui_layer->pixels,
				 ui->ui_layer->width * ui->ui_layer->height * sizeof(int32_t));
		render_ui_element(ui->root, ui->ui_layer);
		mlx_image_to_window(ui->mlx, ui->ui_layer, 0, 0);
		ui->needs_redraw = false;
		
		printf("game_loop: UI updated\n");
	}
	
	return (0);
}

// UI Rendering Functions
void	render_ui_element(t_ui_element *element, mlx_image_t *target)
{
	t_ui_element	*child;
	
	printf("Rendering element: %p, type: %d, pos: (%.0f,%.0f), size: (%.0f,%.0f)\n", 
		  (void *)element, element->type, element->pos.x, element->pos.y, 
		  element->size.x, element->size.y);

	// Draw background
	if (element->style.bg_color & 0xFF000000)
	{
		printf("  Drawing bg rect at (%.0f,%.0f) size (%.0f,%.0f) with color %08X\n", 
			  element->pos.x, element->pos.y, 
			  element->size.x, element->size.y, 
			  element->style.bg_color);
		draw_rect(target, element->pos, element->size, element->style.bg_color);
		if ((element->style.border_color & 0xFF000000))
		{
			const int border_width = 2;
			draw_rect(target, element->pos, 
					 init_v2f(element->size.x, border_width), 
					 element->style.border_color);
			draw_rect(target, 
					 init_v2f(element->pos.x, element->pos.y + element->size.y - border_width), 
					 init_v2f(element->size.x, border_width), 
					 element->style.border_color);
			draw_rect(target, element->pos, 
					 init_v2f(border_width, element->size.y), 
					 element->style.border_color);
			draw_rect(target, 
					 init_v2f(element->pos.x + element->size.x - border_width, element->pos.y), 
					 init_v2f(border_width, element->size.y), 
					 element->style.border_color);
		}
	}
	// Draw element content based on type
	if (element->type == UI_HEADER || element->type == UI_LABEL)
	{
		t_ui_label *label = (t_ui_label *)element->data;
		if (label && label->text)
		{
			int text_width = ft_strlen(label->text) * UI_CHAR_WIDTH;
			int text_height = UI_CHAR_HEIGHT;
			int y_offset = (element->size.y - text_height) / 2 - 2;
			if (y_offset < 0) y_offset = 0;
			t_v2f text_pos = init_v2f(
				element->pos.x + (element->size.x - text_width) / 2,
				element->pos.y + y_offset
			);
			printf("  Drawing %s: '%s' at (%.0f,%.0f) color %08X\n", 
				  (element->type == UI_HEADER) ? "header" : "label",
				  label->text, text_pos.x, text_pos.y, 
				  element->style.text_color);
			draw_text(target, label->text, text_pos, element->style.text_color);
		}
	}
	else if (element->type == UI_BUTTON)
	{
		t_ui_button *button = (t_ui_button *)element->data;
		if (!button) return;
		draw_rect(target, element->pos, element->size, element->style.bg_color);
		if (button->label_text)
		{
			int text_height = UI_CHAR_HEIGHT;
			int y_offset = (element->size.y - text_height) / 2 - 2;
			if (y_offset < 0) y_offset = 0;
			
			t_v2f label_pos = init_v2f(
				element->pos.x + 10,
				element->pos.y + y_offset
			);
			draw_text(target, button->label_text, label_pos, 0xFFFFFFFF);
			if (button->value)
			{
				char value_str[32];
				snprintf(value_str, sizeof(value_str), "%.2f", *button->value);
				int text_width = ft_strlen(value_str) * UI_CHAR_WIDTH;
				int button_width = UI_CHAR_WIDTH * 2;
				int right_padding = 15;
				int value_y_offset = (element->size.y - UI_CHAR_HEIGHT) / 2 - 2;
				if (value_y_offset < 0) value_y_offset = 0;
				t_v2f value_pos = init_v2f(
					element->pos.x + element->size.x - text_width - button_width - right_padding,
					element->pos.y + value_y_offset
				);
				printf("  Button '%s' value: %s, text_width: %d, pos: (%.0f,%.0f)\n", 
					   button->label_text, value_str, text_width, value_pos.x, value_pos.y);
				draw_text(target, value_str, value_pos, 0xFFFFFFFF);
				t_v2f minus_pos = init_v2f(
					value_pos.x - 20,
					value_pos.y
				);
				t_v2f plus_pos = init_v2f(
					value_pos.x + text_width + 10,
					value_pos.y
				);
				draw_text(target, "-", 
					init_v2f(minus_pos.x, minus_pos.y), 
					0x888888FF);
				draw_text(target, "+", 
					init_v2f(plus_pos.x, plus_pos.y + 1),
					0x888888FF);
				printf("  - Buttons at: - (%.0f,%.0f), + (%.0f,%.0f)\n",
					minus_pos.x, minus_pos.y, plus_pos.x, plus_pos.y);
			}
		}
	}
	
	// Render children recursively
	if (element->first_child)
		printf("  Element has children? Yes\n");
	else
		printf("  Element has children? No\n");
	child = element->first_child;
	while (child)
	{
		render_ui_element(child, target);
		child = child->next_sibling;
	}
}

void	handle_ui_click(t_ui_element *root, int32_t x, int32_t y)
{
	t_ui_element	*child;

	if (!root || !is_point_in_element(root, x, y))
		return ;
	if (root->type == UI_BUTTON && root->data)
		update_button_value(root, x);
	child = root->first_child;
	while (child)
	{
		handle_ui_click(child, x, y);
		child = child->next_sibling;
	}
}

bool	is_point_in_element(t_ui_element *element, int32_t x, int32_t y)
{
	return (x >= element->pos.x && 
		x <= (element->pos.x + element->size.x) &&
		y >= element->pos.y && 
		y <= (element->pos.y + element->size.y));
}


void	update_button_value(t_ui_element *button, int32_t click_x)
{
	t_ui_button 	*btn_data;
	t_ui_element	*root;
	bool			value_changed;
	float			relative_x;
	float			button_mid;
	float			old_value;
	t_ui			*ui;

	value_changed = false;
	btn_data = (t_ui_button *)button->data;
	relative_x = click_x - button->pos.x;
	button_mid = button->size.x / 2.0f;
	old_value = *btn_data->value;
	if (relative_x < button_mid)
	{
		*btn_data->value -= btn_data->step;
		value_changed = true;
	}
	else
	{
		*btn_data->value += btn_data->step;
		value_changed = true;
	}
	if (*btn_data->value < btn_data->range.x)
	{
		*btn_data->value = btn_data->range.x;
		if (*btn_data->value == old_value)
			value_changed = false;
	}
	else if (*btn_data->value > btn_data->range.y)
	{
		*btn_data->value = btn_data->range.y;
		if (*btn_data->value == old_value)
			value_changed = false;
	}
	if (value_changed && 
		button->parent && 
		button->parent->parent && 
		button->parent->parent->data &&
		btn_data->value >= &((t_light *)button->parent->parent->data)->color.x &&
		btn_data->value <= &((t_light *)button->parent->parent->data)->color.z + 1)
		*btn_data->value = (int)(*btn_data->value);
root = button;
	while (root->parent)
		root = root->parent;
	if (value_changed && root->data)
	{
		ui = (t_ui *)root->data;
		ui->needs_redraw = true;
	}
}

void	draw_rect(mlx_image_t *img, t_v2f pos, t_v2f size, uint32_t color)
{
	int	dy;
	int	dx;

	dy = 0;
	while (dy < size.y)
	{
		dx = 0;
		while (dx < size.x)
		{
			if (pos.y + dy >= 0 && pos.y + dy < (int)img->height &&
				pos.x + dx >= 0 && pos.x + dx < (int)img->width)
				mlx_put_pixel(img, pos.x + dx, pos.y + dy, color);
			dx++;
		}
		dy++;
	}
}

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
	bool			is_blank;
	int				i;
	int				dy;
	int				dx;
	int				px;
	int				py;

	uc = (unsigned char)c;
	is_blank = true;
	i = 0;
	dy = 0;
	if (uc >= 128 || uc < ' ')
		return;
	while (i < 8)
	{
		if (font[uc][i] != 0)
		{
			is_blank = false;
			break;
		}
		i++;
	}

	if (is_blank)
		return;
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
				{
					mlx_put_pixel(img, px, py, color);
				}
			}
			dx++;
		}
		dy++;
	}
}

void	draw_text(mlx_image_t *img, const char *str, t_v2f pos, uint32_t color)
{
	int		start_x;
	int		char_width;
	int		line_height;
	int		i;

	start_x = pos.x;
	char_width = UI_CHAR_WIDTH;
	line_height = UI_CHAR_HEIGHT;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			pos.x = start_x;
			pos.y += line_height;
			i++;
			continue;
		}
		draw_char(img, str[i], pos.x, pos.y, color);
		pos.x += char_width;
		i++;
		str++;
	}
}

void	*mlx_get_mlx(void)
{
	static mlx_t	*mlx = NULL;

	if (!mlx)
		mlx = mlx_init(800, 600, "MiniRT", true);
	return (mlx);
}

