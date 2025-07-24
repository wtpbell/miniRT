/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_instance.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 20:30:00 by bewong            #+#    #+#             */
/*   Updated: 2025/07/25 00:38:18 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ui.h"
#include "libft.h"
#include "vector.h"
#include <stdio.h>

// Initialize UI images for reuse
t_ui_images *ui_images_create(mlx_t *mlx)
{
	t_ui_images *images;

	printf("ui_images_create: Starting image creation\n");
	printf("ui_images_create: MLX pointer: %p\n", (void *)mlx);

	if (!mlx || !mlx->window)
	{
		printf("ui_images_create: Error - Invalid MLX or MLX window is NULL\n");
		return (NULL);
	}

	printf("ui_images_create: Allocating UI images structure\n");
	images = ft_calloc(1, sizeof(t_ui_images));
	if (!images)
	{
		printf("ui_images_create: Error - Failed to allocate images structure\n");
		return (NULL);
	}
	
	printf("ui_images_create: Creating button image (%dx%d)\n", UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT);
	images->button_img = mlx_new_image(mlx, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT);
	if (!images->button_img)
	{
		printf("ui_images_create: Error - Failed to create button image\n");
		return (free(images), NULL);
	}
	
	printf("ui_images_create: Creating header image (%dx%d)\n", UI_HEADER_WIDTH, UI_HEADER_HEIGHT);
	images->header_img = mlx_new_image(mlx, UI_HEADER_WIDTH, UI_HEADER_HEIGHT);
	if (!images->header_img)
	{
		printf("ui_images_create: Error - Failed to create header image\n");
		mlx_delete_image(mlx, images->button_img);
		return (free(images), NULL);
	}
	
	printf("ui_images_create: Creating panel image (%dx%d)\n", UI_PANEL_WIDTH, mlx->height);
	images->panel_img = mlx_new_image(mlx, UI_PANEL_WIDTH, mlx->height);
	if (!images->panel_img)
	{
		printf("ui_images_create: Error - Failed to create panel image\n");
		mlx_delete_image(mlx, images->button_img);
		mlx_delete_image(mlx, images->header_img);
		return (free(images), NULL);
	}
	
	printf("ui_images_create: All images created successfully at %p\n", (void *)images);
	return (images);
}


void ui_images_destroy(mlx_t *mlx, t_ui_images *images)
{
	if (!images || !mlx)
		return;
	
	if (images->button_img)
		mlx_delete_image(mlx, images->button_img);
	if (images->header_img)
		mlx_delete_image(mlx, images->header_img);
	if (images->panel_img)
		mlx_delete_image(mlx, images->panel_img);
	free(images);
}

// Initialize a UI element with an image
t_ui_element *ui_element_create_with_image(mlx_t *mlx, t_ui_type type, t_v2f pos, t_v2f size)
{
	t_ui_element *element;
	

	if (!mlx || size.x <= 0 || size.y <= 0)
	{
		printf("Error: Invalid parameters for UI element creation\n");
		return NULL;
	}

	element = ft_calloc(1, sizeof(t_ui_element));
	if (!element)
	{
		printf("Error: Failed to allocate memory for UI element\n");
		return NULL;
	}

	element->type = type;
	element->pos = pos;
	element->size = size;
	element->visible = true;
	element->hovered = false;
	element->pressed = false;
	element->parent = NULL;
	element->first_child = NULL;
	element->next_sibling = NULL;
	element->data = NULL;
	element->action = NULL;

	element->image = mlx_new_image(mlx, (uint32_t)size.x, (uint32_t)size.y);
	if (!element->image)
	{
		printf("Error: Failed to create image for UI element\n");
		free(element);
		return NULL;
	}
	element->style = (t_ui_style){
		.bg_color = UI_BG_COLOR,
		.fg_color = 0x00000000,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = 4,
		.visible = true
	};
	if (type == UI_BUTTON || type == UI_VALUE_BUTTON)
	{
		element->style.bg_color = UI_BUTTON_COLOR;
		element->style.fg_color = UI_BUTTON_HOVER_COLOR;
	}
	else if (type == UI_HEADER || type == UI_SECTION)
		element->style.bg_color = UI_HEADER_COLOR;
	else if (type == UI_PANEL)
		element->style.bg_color = UI_PANEL_BG_COLOR;
	
	return (element);
}


void ui_element_render(t_ui_element *element)
{
	if (!element || !element->image || !element->image->pixels)
	{
		printf("Warning: Cannot render element - invalid parameters\n");
		return;
	}e
	if (!element->style.visible)
		return;
	uint8_t *pixels = element->image->pixels;
	size_t pixel_count = element->image->width * element->image->height;
	for (size_t i = 0; i < pixel_count * 4; i += 4)
	{
		pixels[i] = 0;     // B
		pixels[i + 1] = 0; // G
		pixels[i + 2] = 0; // R
		pixels[i + 3] = 0; // A (fully transparent)
	}
	if ((element->style.bg_color & 0xFF000000) != 0)
	{
		for (uint32_t y = 0; y < element->image->height; y++)
		{
			for (uint32_t x = 0; x < element->image->width; x++)
			mlx_put_pixel(element->image, x, y, element->style.bg_color);
		}
		if (element->style.padding > 0)
		{
			for (uint32_t y = 0; y < (uint32_t)element->style.padding; y++)
			{
				for (uint32_t x = 0; x < element->image->width; x++)
					mlx_put_pixel(element->image, x, y, element->style.border_color);
			}
			for (uint32_t y = element->image->height - element->style.padding; y < element->image->height; y++)
			{
				for (uint32_t x = 0; x < element->image->width; x++)
					mlx_put_pixel(element->image, x, y, element->style.border_color);
			}
			for (uint32_t y = 0; y < element->image->height; y++)
			{
				for (uint32_t x = 0; x < (uint32_t)element->style.padding; x++)
					mlx_put_pixel(element->image, x, y, element->style.border_color);
			}
			for (uint32_t y = 0; y < element->image->height; y++)
			{
				for (uint32_t x = element->image->width - element->style.padding; x < element->image->width; x++)
					mlx_put_pixel(element->image, x, y, element->style.border_color);
			}
		}
	}
	if ((element->type == UI_HEADER || element->type == UI_SECTION) && element->data)
	{
		t_ui_label *label = (t_ui_label *)element->data;
		if (label && label->text)
		{
			int text_x = (element->image->width - (ft_strlen(label->text) * 8)) / 2;
			int text_y = (element->image->height - 10) / 2;
			draw_text(element->image, label->text, 
					 init_v2f(text_x + 1, text_y + 1), 
					 0x00000080);
			draw_text(element->image, label->text, 
					 init_v2f(text_x, text_y), 
					 element->style.text_color);
		}
	}
	else if ((element->type == UI_BUTTON || element->type == UI_VALUE_BUTTON) && element->data)
	{
		t_ui_button *button = (t_ui_button *)element->data;
		if (button && button->label_text)
		{
			int text_x = 10;
			int text_y = (element->image->height - 10) / 2;
			draw_text(element->image, button->label_text, 
					 init_v2f(text_x + 1, text_y + 1), 
					 0x00000080);
			draw_text(element->image, button->label_text, 
					 init_v2f(text_x, text_y), 
					 element->style.text_color);
			if (element->type == UI_VALUE_BUTTON && button->value)
			{
				char value_str[32];
				snprintf(value_str, sizeof(value_str), "%.2f", *button->value);
				int value_width = ft_strlen(value_str) * 8;
				draw_text(element->image, value_str, 
						 init_v2f(element->image->width - value_width - 10, text_y), 
						 element->style.text_color);
			}
		}
	}
	if (element->image->instances && element->instance_id >= 0 && 
		element->instance_id < (int32_t)element->image->count)
	{
		element->image->instances[element->instance_id].enabled = 
			element->style.visible && element->visible;
	}
}

int32_t ui_element_add_instance(mlx_t *mlx, t_ui_element *element)
{
	if (!mlx || !element || !element->image)
	{
		printf("Error: Invalid parameters for adding UI instance\n");
		return -1;
	}
	if (element->instance_id < 0)
	{
		ui_element_render(element);
		int32_t instance_id = mlx_image_to_window(mlx, element->image, 
												(int32_t)element->pos.x, 
												(int32_t)element->pos.y);
		if (instance_id < 0)
		{
			printf("Error: Failed to add UI element to window\n");
			return -1;
		}
		element->instance_id = instance_id;
		if (element->image->instances && element->instance_id >= 0)
		{
			element->image->instances[element->instance_id].enabled = 
				element->style.visible && element->visible;
			int32_t depth = 100; // Default depth
			if (element->type == UI_PANEL) depth = 10;
			else if (element->type == UI_SECTION) depth = 20;
			else if (element->type == UI_HEADER) depth = 30;
			else if (element->type == UI_BUTTON || element->type == UI_VALUE_BUTTON) depth = 40;
			mlx_set_instance_depth(&element->image->instances[element->instance_id], depth);
		}
	}
	t_ui_element *child = element->first_child;
	while (child)
	{
		child->pos = init_v2f(element->pos.x + child->pos.x, 
							 element->pos.y + child->pos.y);
		ui_element_add_instance(mlx, child);
		child = child->next_sibling;
	}
	return element->instance_id;
}

void ui_element_set_position(t_ui_element *element, int32_t x, int32_t y)
{
	if (!element || !element->image)
	{
		return;
	}
	t_v2f old_pos = element->pos;
	element->pos = init_v2f((float)x, (float)y);
	if (element->instance_id >= 0 && element->image->instances && 
		element->instance_id < (int32_t)element->image->count)
	{
		element->image->instances[element->instance_id].x = x;
		element->image->instances[element->instance_id].y = y;
		t_ui_element *child = element->first_child;
		while (child)
		{
			ui_element_set_position(child, 
				(int)(element->pos.x + child->pos.x - old_pos.x),
				(int)(element->pos.y + child->pos.y - old_pos.y));
			child = child->next_sibling;
		}
	}
}

// Update element visibility
void ui_element_set_visible(t_ui_element *element, bool visible)
{
	if (!element || !element->image || element->instance_id < 0 || 
		!element->image->instances)
		return;
	
	element->style.visible = visible;
	if (element->instance_id >= 0 && element->instance_id < (int32_t)element->image->count)
	{
		int32_t depth = 0;
		if (element->type == UI_PANEL)
			depth = 0;
		else if (element->type == UI_BUTTON)
			depth = 2;
		else
			depth = 1;
			
		mlx_set_instance_depth(&element->image->instances[element->instance_id], 
			visible ? depth : -1);
	}
	t_ui_element *child = element->first_child;
	while (child)
	{
		ui_element_set_visible(child, visible);
		child = child->next_sibling;
	}
}
