/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:45:51 by bewong            #+#    #+#             */
/*   Updated: 2025/08/06 23:26:23 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file ui.h
 * @brief User Interface module for miniRT
 * 
 * This module provides a canvas-based UI system built on top of MLX42.
 * It supports creating and managing UI elements like panels, buttons, and labels.
 * All UI rendering is done on a single canvas image for better performance.
 */

#ifndef UI_H
# define UI_H

# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include "MLX42/MLX42.h"
# include "vector.h"
# include "scene.h"
# include "game.h"
# include "rt_types.h"
# include "parser.h"

// UI Dimensions
# define UI_PANEL_WIDTH 300
# define UI_BUTTON_WIDTH 30
# define UI_BUTTON_HEIGHT 30

// Ambient Section Layout Ratios
# define UI_LABEL_WIDTH_RATIO 0.3f    // 30% of panel width for labels
# define UI_BTN_WIDTH_RATIO 0.6f   // 60% of panel width for buttons
# define UI_BTN_HEIGHT_RATIO 0.15f // 15% of panel height for button height
# define UI_VERT_PADDING_RATIO 0.04f  // 4% of panel height for vertical padding
# define UI_HORZ_PADDING_RATIO 0.05f  // 5% of panel width for horizontal padding

# define UI_HEADER_HEIGHT 30
# define UI_SECTION_HEADER_HEIGHT 30
# define UI_SECTION_PADDING 10
# define UI_PANEL_PADDING 0
# define UI_PADDING 5
# define UI_CHAR_WIDTH 8
# define UI_CHAR_HEIGHT 15
# define UI_FONT_HEIGHT 8
# define UI_ROW_HEIGHT 30
# define UI_LABEL_PADDING 10
# define UI_LABEL_HEIGHT 20

// UI Colors
# define UI_PANEL_BG_COLOR 0x1E1E2EFF
# define UI_SECTION_COLOR 0x2A2A3AFF
# define UI_BUTTON_COLOR 0x444444FF
# define UI_BUTTON_BORDER_COLOR 0x666666FF
# define UI_TEXT_COLOR 0xFFFFFFFF
# define UI_SECTION_HEADER_COLOR 0x252538FF
# define UI_HEADER_COLOR 0x242437FF
# define UI_BORDER_COLOR 0x3E3E5EFF
# define UI_TRANSPARENT 0x00000000
# define UI_LABEL_COLOR 0xFFFFFFFF

typedef enum e_ui_type
{
	UI_PANEL,       // Container element for grouping other elements
	UI_BUTTON,      // Clickable button with text
	UI_LABEL,       // Text label
	UI_HEADER,      // Section header
	UI_SECTION,     // Section container with header
	UI_VALUE_BUTTON // Button that modifies a numeric value
}	t_ui_type;

typedef struct s_ui_images
{
	mlx_image_t	*button_img;
	mlx_image_t	*header_img;
	mlx_image_t	*panel_img;
}	t_ui_images;

typedef struct s_ui_style
{
	uint32_t	bg_color;        // Background color in 0xRRGGBBAA format
	uint32_t	border_color;    // Border color in 0xRRGGBBAA format
	uint32_t	text_color;      // Text color in 0xRRGGBBAA format
	float		padding;         // Padding in pixels
	bool		visible;         // Visibility flag
}	t_ui_style;

typedef struct s_ui_label
{
	char		*text;
	uint32_t	color;
}	t_ui_label;

typedef struct s_ui_element
{
	t_ui_type			type;           // Type of the UI element
	t_v2f				pos;            // Position relative to parent
	t_v2f				size;           // Size of the element
	t_ui_style			style;          // Visual style properties
	void				*data;          // Type-specific data
	struct s_ui_element	*parent;        // Parent element (NULL for root)
	struct s_ui_element	*first_child;   // First child element
	struct s_ui_element	*next_sibling;  // Next sibling element
	void				(*action)(struct s_ui_element *, void *); // Click action callback
	bool				visible;        // Visibility flag
	t_v2f				abs_pos;        // Absolute position in the window
}	t_ui_element;

typedef struct s_ui_button
{
	void				(*on_click)(t_ui_element *, void *);
	void				*param;
	char				*label;
}	t_ui_button;

typedef struct s_ui_value_button
{
	float				*value;
	float				step;
	t_v2f				range;
	t_ui_element		*value_label;
	void				(*on_click)(t_ui_element *, void *);
	void				*param;
	char				*label;
	char				*(*formatter)(float);
}	t_ui_value_button;

typedef struct s_ui_context
{
	mlx_t				*mlx;           // MLX instance
	t_scene				*scene;         // Reference to the scene data
	mlx_image_t			*canvas;        // Main canvas image for UI rendering
	int					canvas_instance; // Instance ID of the canvas
	t_ui_images			*images;        // Cached images and textures
	bool				is_visible;     // UI visibility flag
	bool				needs_redraw;   // Flag indicating if UI needs redraw
}	t_ui_context;

typedef struct s_ui
{
	t_ui_element	*root;
	t_ui_context	*context;
}	t_ui;

typedef struct s_value_button_config
{
	t_ui_context	*ctx;
	float			*value;
	t_v2f			range;
	float			step;
	t_v2f			pos;
	t_v2f			size;
	char			*(*formatter)(float);
}	t_vbtn_config;

/* UI Context Management */
t_ui_context	*create_ui_context(mlx_t *mlx, t_scene *scene);
void			destroy_ui_context(t_ui_context *ctx);
t_ui			*create_ui(mlx_t *mlx, t_scene *scene);
void			destroy_ui(t_ui *ui);
void			render_ui(t_ui *ui);
void			toggle_ui_visibility(t_ui *ui);

/* UI Element Creation */
t_ui_element	*create_ui_element(t_ui_type type, t_v2f pos, t_v2f size);
t_ui_element	*create_header(t_ui_context *ctx, const char *title, t_v2f pos, t_v2f size);
t_ui_element	*create_panel(t_ui_context *ctx, t_v2f pos, t_v2f size);
t_ui_element	*create_button(t_ui_context *ctx, const char *label, t_v2f pos, t_v2f size,
					void (*on_click)(t_ui_element *, void *), void *param);
t_ui_element	*create_label(t_ui_context *ctx, const char *text, t_v2f pos, uint32_t color);
t_ui_element	*create_labeled_control(t_vbtn_config *cfg, const char *label_text, float total_width);
t_ui_element	*create_ambient_section(t_ui_context *ctx, t_scene *scene, t_v2f pos, t_v2f size);
t_ui_element	*create_camera_section(t_ui_context *ctx, t_scene *scene, t_v2f pos, t_v2f size);
t_ui_element	*create_light_section(t_ui_context *ctx, t_scene *scene, t_v2f pos, t_v2f size);
t_ui_element	*create_ui_sections(t_ui_context *ctx, t_scene *scene, t_v2f pos, t_v2f size);

/* UI Element Management */
void			destroy_ui_element(t_ui_element *element, t_ui_context *ctx);
void			destroy_ui_element_recursive(t_ui_element *element, t_ui_context *ctx);
bool			ui_element_remove_child(t_ui_element *parent, t_ui_element *child, bool destroy, t_ui_context *ctx);
void			attach_child(t_ui_element *parent, t_ui_element *child);
void			update_button_value(t_ui_element *button, int32_t click_x, t_ui_context *ctx);
void			update_label_text(t_ui_element *label, const char *text, t_ui_context *ctx);
void			handle_ui_click(t_ui_element *root, int32_t x, int32_t y, t_ui_context *ctx);
/* UI Rendering */
void			render_ui_element(t_ui_element *element, t_ui_context *ctx);
void			draw_button(t_ui_element *button, t_ui_context *ctx);
void			draw_char(mlx_image_t *img, char c, int x, int y, uint32_t color);
void			draw_text(mlx_image_t *img, const char *str, t_v2f pos, uint32_t color);
void			draw_rect(mlx_image_t *img, t_v2f pos, t_v2f size, uint32_t color);
void			draw_rect_border(mlx_image_t *img, t_v2f pos, t_v2f size, uint32_t color);

/* UI Helpers */
char			*format_float_value(float value);
char			*format_color_value(float value);
t_light			*find_light(t_scene *scene, t_light_type type);
void			decrement_value_button(t_ui_element *btn, void *param);
void			increment_value_button(t_ui_element *btn, void *param);
void			init_value_button_data(t_ui_value_button *value_btn, const t_vbtn_config *cfg);
void			add_inc_dec_buttons(t_ui_element *container, const t_vbtn_config *cfg, t_ui_value_button *value_btn);
void			add_value_label(t_ui_element *container, const t_vbtn_config *cfg, t_ui_value_button *value_btn);

/* Default Styling */
void			default_button(t_ui_element *button, t_v2f pos, t_v2f size);
void			default_value_button(t_ui_element *button, t_v2f pos, t_v2f size, const char *label);
void			default_section(t_ui_element *section, t_v2f pos, t_v2f size);
void			default_header(t_ui_element *header, t_v2f pos, t_v2f size);
void			default_panel(t_ui_element *panel, t_v2f pos, t_v2f size);
void			default_label(t_ui_element *label, t_v2f pos, t_v2f size);

/* Utility */
char			*ft_ftoa(float f, int precision);
uint32_t		blend_colors(uint32_t bg, uint32_t fg);

#endif
