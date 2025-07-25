/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   new_ui.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/23 14:48:38 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/25 17:51:56 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// UI Constants (only define those not in ui.h)
#define UI_SECTION_PADDING 10
#define UI_SECTION_SPACING 10


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
// └── BUTTON "Submit" (action)

// /* Helper function to initialize button data */
// static t_ui_button *init_button(const char *label, float *value, 
// 							   float min, float max, float step)
// {
// 	t_ui_button *btn = (t_ui_button *)malloc(sizeof(t_ui_button));
// 	if (!btn) return NULL;
	
// 	btn->label_text = ft_strdup(label);
// 	btn->value = value;
// 	btn->step = step;
// 	btn->range.x = min;
// 	btn->range.y = max;
// 	btn->is_hovered = false;
// 	btn->instance_id = -1;
// 	btn->img = NULL;
// 	btn->on_click = NULL;
// 	return btn;
// }


/* Create a new UI element */
t_ui_element *ui_element_create(t_ui_type type, t_v2f pos, t_v2f size)
{
	t_ui_element *element = (t_ui_element *)malloc(sizeof(t_ui_element));
	if (!element) return NULL;

	// Initialize common properties
	element->type = type;
	element->pos = pos;
	element->size = size;
	element->parent = NULL;
	element->first_child = NULL;
	element->next_sibling = NULL;
	element->data = NULL;
	element->action = NULL;
	element->visible = true;
	element->image = NULL;
	element->instance_id = -1;

	// Set default styles
	element->style = (t_ui_style){
		.bg_color = 0xFF2D2D2D,     // Dark gray background
		.fg_color = 0xFFFFFFFF,     // White foreground
		.text_color = 0xFFFFFFFF,   // White text
		.border_color = 0xFF555555, // Light gray border
		.padding = 8,
		.visible = true
	};

	if (type == UI_PANEL)
	{
		element->style.bg_color = 0xFF2D2D2D;
		element->style.border_color = 0xFF3F3F46;
		element->style.padding = 10;
	}
	else if (type == UI_SECTION)
	{
		element->style.bg_color = 0xFF32353B;
		element->style.border_color = 0xFF4A4D54;
		element->style.padding = 8;
	}
	else if (type == UI_HEADER)
	{
		element->style.bg_color = 0xFF1E88E5;
		element->style.text_color = 0xFFFFFFFF;
		element->style.border_color = 0xFF64B5F6;
		element->style.padding = 12;
	}
	else if (type == UI_BUTTON)
	{
		element->style.bg_color = 0xFF3E4146;
		element->style.text_color = 0xFFFFFFFF;
		element->style.border_color = 0xFF646C7A;
		element->style.padding = 6;
	}
	else if (type == UI_VALUE_BUTTON)
	{
		element->style.bg_color = 0xFF3A3E44;
		element->style.text_color = 0xFFFFFFFF;
		element->style.border_color = 0xFF5D646F;
		element->style.padding = 6;
	}
	else if (type == UI_LABEL)
	{
		element->style.bg_color = UI_TRANSPARENT;  // Transparent
		element->style.text_color = 0xFFFFFFFF; // White text
		element->style.padding = 2;
	}
	return (element);
}

void ui_add_child(t_ui_element *parent, t_ui_element *child)
{
	if (!parent || !child) return;
	child->parent = parent;
	if (!parent->first_child)
	{
		parent->first_child = child;
		return;
	}
	t_ui_element *sibling = parent->first_child;
	while (sibling->next_sibling) {
		sibling = sibling->next_sibling;
	}
	sibling->next_sibling = child;
}

/* Create a button element with the specified properties */
t_ui_element *create_button(mlx_t *mlx, const char *label, t_v2f pos, t_v2f size,
						  void (*on_click)(t_ui_element *, void *), void *param)
{
	t_ui_element *button = ui_element_create(UI_BUTTON, pos, size);
	if (!button) return NULL;
	button->action = on_click;
	t_ui_button *btn_data = (t_ui_button *)malloc(sizeof(t_ui_button));
	if (btn_data) {
		btn_data->label_text = ft_strdup(label);
		btn_data->param = param;
		button->data = btn_data;
	}
	button->image = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (button->image) {
		uint32_t *pixels = (uint32_t *)button->image->pixels;
		for (size_t i = 0; i < (size_t)(size.x * size.y); i++) {
			pixels[i] = button->style.bg_color;
		}
		for (uint32_t x = 0; x < (uint32_t)size.x; x++) {
			pixels[x] = button->style.border_color; // Top border
			pixels[((uint32_t)size.y - 1) * (uint32_t)size.x + x] = button->style.border_color; // Bottom border
		}
		for (uint32_t y = 0; y < (uint32_t)size.y; y++) {
			pixels[y * (uint32_t)size.x] = button->style.border_color; // Left border
			pixels[y * (uint32_t)size.x + (uint32_t)size.x - 1] = button->style.border_color; // Right border
		}
		t_v2f text_pos = init_v2f(
			(size.x - (ft_strlen(label) * UI_CHAR_WIDTH)) / 2,
			(size.y - UI_CHAR_HEIGHT) / 2
		);
		draw_text(button->image, label, text_pos, button->style.text_color);
	}
	
	return button;
}

t_ui_element *create_panel(mlx_t *mlx, t_v2f pos, t_v2f size)
{
	t_ui_element *panel;
	
	printf("create_panel: Creating panel at (%.1f, %.1f) with size (%.1f, %.1f)\n", 
		   pos.x, pos.y, size.x, size.y);
	
	panel = (t_ui_element *)malloc(sizeof(t_ui_element));
	if (!panel)
	{
		printf("create_panel: Failed to allocate memory for panel\n");
		return (NULL);
	}
	
	panel = ui_element_create_with_image(mlx, UI_PANEL, pos, size);
	if (!panel->image)
	{
		printf("create_panel: Failed to create panel image\n");
		free(panel);
		return (NULL);
	}
	panel->type = UI_PANEL;
	panel->pos = pos;
	panel->size = size;
	panel->visible = true;
	panel->parent = NULL;
	panel->first_child = NULL;
	panel->next_sibling = NULL;
	panel->instance_id = -1;
	
	// Customize panel style
	panel->style.bg_color = UI_PANEL_BG_COLOR;
	panel->style.text_color = UI_TEXT_COLOR;
	panel->style.border_color = UI_BORDER_COLOR;
	panel->style.padding = 10;
	panel->style.visible = true;
	
	printf("create_panel: Panel created at %p with image at %p\n", 
		   (void *)panel, (void *)panel->image);
	
	panel->instance_id = ui_element_add_instance(mlx, panel);
	if (panel->instance_id < 0)
	{
		printf("create_panel: Failed to add panel instance to window\n");
		mlx_delete_image(mlx, panel->image);
		free(panel);
		return (NULL);
	}
	
	printf("create_panel: Panel instance ID: %d\n", panel->instance_id);
	
	return (panel);
}

t_ui_element *create_section(mlx_t *mlx, const char *title, t_v2f pos, t_v2f size)
{
	t_ui_element *section;
	
	printf("create_section: Creating section at (%.1f, %.1f) with size (%.1f, %.1f)\n", 
		   pos.x, pos.y, size.x, size.y);
	section = ui_element_create_with_image(mlx, UI_SECTION, pos, size);
	if (!section)
	{
		printf("Error: Failed to create section element\n");
		return (NULL);
	}
	printf("create_section: Section created at %p\n", (void *)section);
	section->style = (t_ui_style){
		.bg_color = 0x2A2D32FF,       // Dark gray background
		.fg_color = UI_TRANSPARENT,       // Not used
		.text_color = 0xFFFFFFFF,     // White text
		.border_color = 0x3F3F46FF,   // Slightly lighter border
		.padding = 8,
		.visible = true
	};

	if (title && mlx)
	{
		printf("create_section: Creating header for section\n");
		t_v2f header_size = init_v2f(size.x, 30); // Fixed header height
		t_ui_element *header = create_header(mlx, title, init_v2f(0, 0), header_size);
		printf("create section: done header\n");
		if (header)
		{
			// printf("create_section: Header created at %p\n", (void *)header);
			// Style the header
			header->style.bg_color = 0x1E88E5FF;  // Blue header
			header->style.text_color = 0xFFFFFFFF; // White text
			header->style.border_color = 0x64B5F6FF; // Lighter blue border
			header->style.padding = 10;
			
			printf("create_section: Adding header as child to section\n");
			ui_add_child(section, header);
		}
		else
		{
			printf("Warning: Failed to create header for section\n");
		}
	}
	printf("create_section: Rendering section\n");
	ui_element_render(section);
	if (mlx)
	{
		printf("create_section: Adding section to MLX window\n");
		section->instance_id = ui_element_add_instance(mlx, section);
		if (section->instance_id < 0)
		{
			printf("Error: Failed to add section to window\n");
			return (NULL);
		}
		printf("create_section: Section added to window with instance ID %d\n", section->instance_id);
	}
	
	printf("create_section: Section creation completed successfully\n");

	return (section);
}

t_ui_element *create_header(mlx_t *mlx, const char *title, t_v2f pos, t_v2f size)
{
	t_ui_element *header;
	
	header = ui_element_create_with_image(mlx, UI_HEADER, pos, size);
	if (!header)
		return (NULL);
	header->style = (t_ui_style){
		.bg_color = 0x1E88E5FF,       // Blue background
		.fg_color = UI_TRANSPARENT,       // Not used
		.text_color = 0xFFFFFFFF,     // White text
		.border_color = 0x64B5F6FF,   // Lighter blue border
		.padding = 10,
		.visible = true
	};
	
	printf("create_header:  in title\n");
	if (title)
	{
		t_v2f label_pos = init_v2f(
			(size.x - (ft_strlen(title) * 8)) / 2,  // Center horizontally
			(size.y - 10) / 2                       // Center vertically
		);
		
		t_ui_element *label_elem = create_label(mlx, title, label_pos, 0xFFFFFFFF);
		if (label_elem)
		{
			label_elem->style.bg_color = UI_TRANSPARENT;  // Transparent background
			label_elem->style.text_color = 0xFFFFFFFF; // White text
			label_elem->style.padding = 0;
			ui_add_child(header, label_elem);
			t_ui_label *label_data = (t_ui_label *)malloc(sizeof(t_ui_label));
			if (label_data)
			{
				label_data->text = ft_strdup(title);
				label_data->color = 0xFFFFFFFF; // White text
				header->data = label_data;
			}
		}
	}
	printf("create_header: finished title\n");
	ui_element_render(header);
	if (mlx)
	{
		header->instance_id = ui_element_add_instance(mlx, header);
		if (header->instance_id < 0)
		{
			if (header->data) {
				t_ui_label *label = (t_ui_label *)header->data;
				if (label->text) free(label->text);
				free(header->data);
			}
			free(header);
			return (NULL);
		}
		if (header->image && header->instance_id >= 0 &&
			header->instance_id < (int32_t)header->image->count)
		{
			mlx_set_instance_depth(&header->image->instances[header->instance_id], 200);
		}
		printf("create_header: done mlx\n");
	}
	printf("create_header: finished header\n");
	return (header);
}

t_ui_element *create_label(mlx_t *mlx, const char *title, t_v2f pos, uint32_t color)
{
	t_ui_element *label;
	t_ui_label *label_data;

	if (!mlx)
	{
		printf("Error: MLX context is required for label creation\n");
		return (NULL);
	}

	// Create label with image
	label = ui_element_create_with_image(mlx, UI_LABEL, pos, 
		init_v2f(UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT));
	if (!label)
	{
		printf("Error: Failed to create label image\n");
		return (NULL);
	}

	// Set label style
	label->style = (t_ui_style){
		.bg_color = UI_TRANSPARENT,  // Transparent background
		.fg_color = UI_TRANSPARENT,  // No foreground color
		.text_color = color,     // Text color from parameter
		.border_color = UI_TRANSPARENT, // No border
		.padding = 2,
		.visible = true
	};

	// Create and set up label data
	label_data = malloc(sizeof(t_ui_label));
	if (!label_data)
	{
		destroy_ui_element_recursive(label, NULL);
		return (NULL);
	}

	label_data->text = ft_strdup(title);
	if (!label_data->text)
	{
		free(label_data);
		destroy_ui_element_recursive(label, NULL);
		return (NULL);
	}

	label_data->color = color;
	label->data = label_data;

	// Render the label with its current style
	ui_element_render(label);

	// Add to window and store instance ID
	label->instance_id = ui_element_add_instance(mlx, label);
	if (label->instance_id < 0)
	{
		printf("Error: Failed to add label to window\n");
		destroy_ui_element_recursive(label, NULL);
		return (NULL);
	}

	return (label);
}

t_ui_element *create_value_button(mlx_t *mlx, const char *label, float *value,
							   t_v2f range, float step, t_v2f pos, t_v2f size)
{
	t_ui_element *button;
	t_ui_button *btn;
	char value_text[32];
	t_v2f text_pos;

	if (!mlx || !label || !value)
	{
		printf("Error: Invalid parameters for value button\n");
		return (NULL);
	}
	button = ui_element_create_with_image(mlx, UI_VALUE_BUTTON, pos, size);
	if (!button)
	{
		printf("Error: Failed to create value button image\n");
		return (NULL);
	}
	button->style = (t_ui_style){
		.bg_color = UI_BUTTON_COLOR,
		.fg_color = UI_BUTTON_HOVER_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = 5,
		.visible = true
	};
	btn = malloc(sizeof(t_ui_button));
	if (!btn)
	{
		free(button);
		return (NULL);
	}
	btn->label_text = ft_strdup(label);
	if (!btn->label_text)
	{
		free(btn);
		free(button);
		return (NULL);
	}
	btn->value = value;
	btn->range = range;
	btn->step = step;
	btn->instance_id = -1;
	*value = fmax(range.x, fmin(range.y, *value));
	button->data = btn;
	ui_element_render(button);
	if (label) {
		text_pos = init_v2f(button->style.padding, 
						  (button->size.y - UI_CHAR_HEIGHT) / 2);
		draw_text(button->image, label, text_pos, button->style.text_color);
	}
	snprintf(value_text, sizeof(value_text), "%.2f", *value);
	text_pos = init_v2f(button->size.x - 60, 
					  (button->size.y - UI_CHAR_HEIGHT) / 2);
	draw_text(button->image, value_text, text_pos, button->style.text_color);
	button->instance_id = ui_element_add_instance(mlx, button);
	if (button->instance_id < 0)
	{
		printf("Error: Failed to add button to window\n");
		destroy_ui_element_recursive(button, NULL);
		return (NULL);
	}

	return (button);
}

// static void	position_vertically(t_ui_element *element, t_v2f pos, float spacing)
// {
// 	if (!element)
// 		return;
// 	element->pos = pos;
// 	if (element->next_sibling)
// 	{
// 		t_v2f next_pos = init_v2f(pos.x, pos.y + element->size.y + spacing);
// 		position_vertically(element->next_sibling, next_pos, spacing);
// 	}
// }

t_ui_element *create_ambient_section(mlx_t *mlx, t_scene *scene, t_v2f pos, t_v2f size)
{
	printf("create_ambient_section: Starting with scene at %p\n", (void *)scene);
	(void)size;
	// === Step 1: Create section with calculated size ===
	const float header_height = 30.0f;
	const float button_height = 30.0f;
	const float button_spacing = 10.0f;
	const float padding = 10.0f;
	const int num_controls = 4;

	float section_height = header_height + (num_controls * (button_height + button_spacing)) + padding;
	t_v2f section_size = init_v2f(UI_PANEL_WIDTH - 20, section_height);

	t_ui_element *section = create_section(mlx, "AMBIENT", pos, section_size);
	if (!section)
	{
		printf("create_ambient_section: Failed to create section\n");
		return NULL;
	}
	printf("create_ambient_section: Section created at %p\n", (void *)section);

	// === Step 2: Try to find content panel inside section ===
	t_ui_element *content = section->first_child ? section->first_child->next_sibling : NULL;
	if (!content)
	{
		printf("create_ambient_section: Warning: content panel not found, using section directly\n");
		content = section; // fallback
	}

	// === Step 3: Find or create ambient light ===
	t_light *am_light = NULL;
	for (int i = 0; i < scene->lights.size; i++)
	{
		t_light *light = (t_light *)scene->lights.items[i];
		if (!light)
			continue;
		if (light->type == LIGHT_AMBIENT)
		{
			am_light = light;
			break;
		}
	}
	if (!am_light)
	{
		printf("create_ambient_section: No ambient light found, creating default\n");
		am_light = (t_light *)malloc(sizeof(t_light));
		if (!am_light)
		{
			printf("create_ambient_section: Failed to allocate ambient light\n");
			return section;
		}
		am_light->type = LIGHT_AMBIENT;
		am_light->color = init_v3f(1.0f, 1.0f, 1.0f);
		am_light->intensity = 0.2f;
	}

	// === Step 4: Create buttons ===
	t_v2f color_range = init_v2f(0, 255);
	t_v2f intensity_range = init_v2f(0, 1);
	t_v2f button_size = init_v2f(section_size.x - 2 * padding, button_height);
	t_v2f button_pos = init_v2f(padding, header_height + padding);

	t_ui_element *col_r = create_value_button(mlx, "COL R", &am_light->color.x, color_range, 5.0f, button_pos, button_size);
	button_pos.y += button_height + button_spacing;

	t_ui_element *col_g = create_value_button(mlx, "COL G", &am_light->color.y, color_range, 5.0f, button_pos, button_size);
	button_pos.y += button_height + button_spacing;

	t_ui_element *col_b = create_value_button(mlx, "COL B", &am_light->color.z, color_range, 5.0f, button_pos, button_size);
	button_pos.y += button_height + button_spacing;

	t_ui_element *am_inten = create_value_button(mlx, "INTENSITY", &am_light->intensity, intensity_range, 0.1f, button_pos, button_size);

	if (!col_r || !col_g || !col_b || !am_inten)
	{
		printf("Error: Failed to create ambient section controls\n");
		if (col_r) destroy_ui_element_recursive(col_r, NULL);
		if (col_g) destroy_ui_element_recursive(col_g, NULL);
		if (col_b) destroy_ui_element_recursive(col_b, NULL);
		if (am_inten) destroy_ui_element_recursive(am_inten, NULL);
		destroy_ui_element_recursive(section, NULL);
		return NULL;
	}

	// === Step 5: Add buttons to content panel ===
	ui_add_child(content, col_r);
	ui_add_child(content, col_g);
	ui_add_child(content, col_b);
	ui_add_child(content, am_inten);

	section->data = am_light;

	return section;
}


int game_loop(void *param)
{
	static bool first_frame = true;
	t_game *game = (t_game *)param;
	t_ui *ui = game->ui;
	bool needs_scene_redraw = false;

	needs_scene_redraw = first_frame || ui->needs_redraw;
	first_frame = false;

	// Always render the UI
	if (ui) {
		// Clear the UI image first
		if (ui->ui_image) {
			ft_memset(ui->ui_image->pixels, 0, 
				ui->ui_image->width * ui->ui_image->height * sizeof(int32_t));
		}

		// Render all UI elements
		t_ui_element *current = ui->root;
		while (current) {
			render_ui_element(current, ui->ui_image);
			current = current->next_sibling;
		}

		// Mark UI for redraw
		ui->needs_redraw = false;
	}

	// Render scene if needed
	if (needs_scene_redraw) {
		render_scene(game);
	}
	
	return (0);
}

// UI Rendering Functions
void	render_ui_element(t_ui_element *element, mlx_image_t *target)
{
	t_ui_element	*child;
	
	if (!element || !target || !element->style.visible) {
		if (!element || !target) {
			printf("render_ui_element: Invalid parameters - element: %p, target: %p\n", 
				  (void *)element, (void *)target);
		}
		return;
	}

	// Draw background if not fully transparent
	if ((element->style.bg_color & 0xFF000000) != 0) {
		t_v2f draw_pos = element->pos;
		t_v2f draw_size = element->size;
		
		// Clamp to target bounds
		draw_pos.x = (draw_pos.x < 0) ? 0 : draw_pos.x;
		draw_pos.y = (draw_pos.y < 0) ? 0 : draw_pos.y;
		draw_size.x = (draw_pos.x + draw_size.x > target->width) ? 
			target->width - draw_pos.x : draw_size.x;
		draw_size.y = (draw_pos.y + draw_size.y > target->height) ? 
			target->height - draw_pos.y : draw_size.y;
		
		// Only draw if we have positive dimensions
		if (draw_size.x > 0 && draw_size.y > 0) {
			// Draw background
			draw_rect(target, draw_pos, draw_size, element->style.bg_color);
			
			// Draw border if not fully transparent
			if ((element->style.border_color & 0xFF000000) != 0) {
				const int border = 2; // Slightly thicker border for better visibility
				// Top border
				draw_rect(target, draw_pos, 
					 (init_v2f)(draw_size.x, border), 
					 element->style.border_color);
				// Bottom border
				draw_rect(target, 
					 (init_v2f)(draw_pos.x, draw_pos.y + draw_size.y - border), 
					 (init_v2f)(draw_size.x, border), 
					 element->style.border_color);
				// Left border
				draw_rect(target, draw_pos, 
					 (init_v2f)(border, draw_size.y), 
					 element->style.border_color);
				// Right border
				draw_rect(target, 
					 (init_v2f)(draw_pos.x + draw_size.x - border, draw_pos.y), 
					 (init_v2f)(border, draw_size.y), 
					 element->style.border_color);
			}
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
			int y_offset = (element->size.y - text_height) / 2;
			if (y_offset < 0) y_offset = 0;
			
			t_v2f text_pos = init_v2f(
				element->pos.x + (element->size.x - text_width) / 2,
				element->pos.y + y_offset + 2  // Add small vertical offset
			);
			
			// Ensure text color has full alpha if not set
			uint32_t text_color = element->style.text_color | 0xFF000000;
			
			printf("  Drawing %s: '%s' at (%.0f,%.0f) color %08X\n", 
				  (element->type == UI_HEADER) ? "header" : "label",
				  label->text, text_pos.x, text_pos.y, 
				  text_color);
			  
			// Draw text shadow for better visibility
			t_v2f shadow_pos = init_v2f(text_pos.x + 1, text_pos.y + 1);
			draw_text(target, label->text, shadow_pos, 0x000000FF);
			
			// Draw main text
			draw_text(target, label->text, text_pos, text_color);
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

	if (!img || !str) {
		printf("draw_text: Invalid parameters - img: %p, str: %p\n", (void *)img, (void *)str);
		return;
	}

	start_x = (int)pos.x;
	char_width = UI_CHAR_WIDTH;
	line_height = UI_CHAR_HEIGHT + 2; // Add some spacing between lines

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
		
		// Only draw if the character is within the image bounds
		if (pos.x >= 0 && pos.x < (int)img->width - char_width &&
			pos.y >= 0 && pos.y < (int)img->height - line_height)
		{
			// Draw text shadow for better readability
			draw_char(img, str[i], (int)pos.x + 1, (int)pos.y + 1, 0x000000FF);
			// Draw main text
			draw_char(img, str[i], (int)pos.x, (int)pos.y, color);
		}
		pos.x += char_width;
		i++;
	}
}

void	*mlx_get_mlx(void)
{
	static mlx_t	*mlx = NULL;

	if (!mlx)
		mlx = mlx_init(800, 600, "MiniRT", true);
	return (mlx);
}

