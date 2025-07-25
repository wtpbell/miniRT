/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui_layout.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/25 11:39:13 by bewong        #+#    #+#                 */
/*   Updated: 2025/07/25 17:51:56 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

void layout_children(t_ui_element *parent)
{
	t_ui_element *child = parent->first_child;
	float cursor_y = parent->style.padding;
	int padding = parent->style.padding;

	while (child)
	{
		if (child->visible && child->style.visible && child->image && child->instance_id >= 0)
		{
			mlx_instance_t *inst = &child->image->instances[child->instance_id];
			inst->x = (int)(parent->pos.x + padding);
			inst->y = (int)(parent->pos.y + cursor_y);
			child->pos.x = padding;
			child->pos.y = cursor_y;
			cursor_y += child->size.y + padding;
		}
		child = child->next_sibling;
	}
}

void attach_child(t_ui_element *parent, t_ui_element *child)
{
	if (!parent || !child)
		return;
	child->parent = parent;
	child->next_sibling = parent->first_child;
	parent->first_child = child;
	layout_children(parent);
}

// Example usage in button creation
void apply_instance(t_ui_element *element, mlx_image_t *img, mlx_t *mlx, int32_t x, int32_t y)
{
	if (!element || !img || !mlx)
		return;
	if (mlx_image_to_window(mlx, img, x, y) < 0)
	{
		printf("Failed to attach image to window\n");
		return;
	}
	element->image = img;
	element->instance_id = img->count - 1;
}

t_ui_element	*create_panel(t_ui *ui, t_v2f pos, t_v2f size)
{
	t_ui_element	*panel;
	
	panel = ft_calloc(1, sizeof(t_ui_element));
	if (!panel)
		return (NULL);
	default_panel(&panel, pos, size);
	if (ui && ui->images && ui->images->panel_img)
		apply_instance(panel, ui->images->panel_img, ui->mlx, (int)pos.x, (int)pos.y);
	return (panel);
}

t_ui_element *create_label(t_ui *ui, const char *title, t_v2f pos, t_v2f size)
{
	t_ui_element	*label;
	t_ui_label		*label_data;
	
	label = ft_calloc(1, sizeof(t_ui_element));
	if (!label)
		return (NULL);
	default_label(&label, pos, size);
	label_data = ft_calloc(1, sizeof(t_ui_label));
	if (!label_data)
		return (free(label), NULL);
	label_data->text = ft_strdup(title);
	if (!label_data->text)
		return (free(label), free(label_data), NULL);
	label_data->color = UI_TRANSPARENT;
	label->data = label_data;
	return (label);
}

t_ui_element	*create_header(t_ui *ui, const char *title, t_v2f pos, t_v2f size)
{
	t_ui_element	*header;
	t_ui_element	*label;
	t_v2f			label_pos;

	header = ft_calloc(1, sizeof(t_ui_element));
	if (!header)
		return (NULL);
	
	if (ui && ui->images && ui->images->header_img)
		apply_instance(header, ui->images->header_img, ui->mlx, (int)pos.x, (int)pos.y);
	if (title)
	{
		label = create_label(ui, title, init_v2f(0, 0), inti_v2f());
		if (label)
			attach_child(header, label);
	}
	return (header);
}

t_ui_element *create_value_button(t_ui *ui, const char *label, float *value,
								  t_v2f range, float step, t_v2f pos, t_v2f size)
{
	t_ui_element	*button;
	t_ui_value_button	*val_btn;

	button = ft_calloc(1, sizeof(t_ui_element));
	if (!button)
		return (NULL);
	button->type = UI_VALUE_BUTTON;
	button->pos = pos;
	button->size = size;
	button->visible = true;
	button->instance_id = -1;
	button->style = (t_ui_style){
		.bg_color = UI_BUTTON_COLOR,
		.fg_color = UI_BUTTON_HOVER_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = 5,
		.visible = true
	};
	if (ui->images && ui->images->button_img)
		apply_instance(button, ui->images->button_img, ui->mlx, (int)pos.x, (int)pos.y);

	val_btn = ft_calloc(1, sizeof(t_ui_value_button));
	if (!val_btn)
		return (free(button), NULL);

	val_btn->value = value;
	val_btn->range = range;
	val_btn->step = step;
	val_btn->on_click = NULL;
	val_btn->param = NULL;

	// Clamp value within range
	*value = fmaxf(range.x, fminf(range.y, *value));

	button->data = val_btn;

	// Create label for button text
	t_ui_element *label_elem = create_label(ui, label, init_v2f(0, 0), button->style.text_color);
	if (label_elem)
		attach_child(button, label_elem);

	// Create label for value text and store pointer in val_btn
	char value_str[32];
	snprintf(value_str, sizeof(value_str), "%.2f", *value);
	t_ui_element *value_elem = create_label(ui, value_str, init_v2f(0, 0), button->style.text_color);
	if (value_elem)
	{
		attach_child(button, value_elem);
		val_btn->value_label = value_elem;
	}

	return button;
}


t_ui_element *create_button(t_ui *ui, const char *label, t_v2f pos, t_v2f size,
							void (*on_click)(t_ui_element *, void *), void *param)
{
	t_ui_element *button;
	t_ui_button *btn;

	button = ft_calloc(1, sizeof(t_ui_element));
	if (!button)
		return (NULL);

	button->type = UI_BUTTON;
	button->pos = pos;
	button->size = size;
	button->visible = true;
	button->instance_id = -1;
	button->style = (t_ui_style){
		.bg_color = UI_BUTTON_COLOR,
		.fg_color = UI_BUTTON_HOVER_COLOR,
		.text_color = UI_TEXT_COLOR,
		.border_color = UI_BORDER_COLOR,
		.padding = 5,
		.visible = true
	};

	if (ui->images && ui->images->button_img)
		apply_instance(button, ui->images->button_img, ui->mlx, (int)pos.x, (int)pos.y);

	btn = ft_calloc(1, sizeof(t_ui_button));
	if (!btn)
		return (free(button), NULL);

	btn->on_click = on_click;
	btn->param = param;
	btn->label_text = ft_strdup(label);
	button->data = btn;

	// Create label for button text
	t_ui_element *label_elem = create_label(ui, label, init_v2f(0, 0), button->style.text_color);
	if (label_elem)
		attach_child(button, label_elem);

	return button;
}
