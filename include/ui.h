/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:45:51 by bewong            #+#    #+#             */
/*   Updated: 2025/07/31 00:23:03 by bewong           ###   ########.fr       */
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


/* Forward declarations */
typedef struct s_ui t_ui;
typedef struct s_ui_context t_ui_context;
typedef struct s_ui_element t_ui_element;

typedef void (*t_ui_destroy_func)(t_ui_element *element, t_ui_context *ctx);
typedef void (*t_ui_render_func)(t_ui_element *, t_ui_context *);
typedef void (*t_ui_click_func)(t_ui_element *element, int x, int y, void *param);
typedef void (*t_ui_update_func)(t_ui_element *, void *);

# define WIDTH 1600
# define HEIGHT 900
// UI Dimensions
# define UI_PANEL_WIDTH 300
# define UI_BUTTON_WIDTH 200

// Value Button Constants
# define VALUE_BTN_BUTTON_WIDTH 30      // Width of + and - buttons
# define VALUE_BTN_PADDING 5            // Padding around buttons
# define VALUE_BTN_MIN_WIDTH 100        // Minimum width for the value button container
# define VALUE_BTN_DEFAULT_HEIGHT 30    // Default height for the value button
# define UI_BUTTON_HEIGHT 30
# define UI_BUTTON_SPACING 10
# define UI_HEADER_HEIGHT 30
# define UI_HEADER_WIDTH 230
# define UI_SECTION_HEADER_HEIGHT 30
# define UI_SECTION_PADDING 10
# define UI_PANEL_PADDING 4
# define UI_SECTION_SPACING 10
# define UI_CHAR_WIDTH 8
# define UI_CHAR_HEIGHT 15
# define UI_FONT_HEIGHT 8

// UI Layer Depths
# define UI_LAYER_PANEL 10
# define UI_LAYER_SECTION 20
# define UI_LAYER_HEADER 30
# define UI_LAYER_BUTTON 40
# define UI_LAYER_LABEL 50
# define UI_SECTION_WIDTH 230
# define UI_CHAR_WIDTH 8
# define UI_CHAR_HEIGHT 15

// UI Colors
# define UI_BG_COLOR 0x11111180
# define UI_PANEL_BG_COLOR 0x1E1E2EFF  // Dark blue-gray panel
# define UI_SECTION_COLOR 0x2A2A3AFF   // Slightly lighter blue-gray for sections
# define UI_BUTTON_COLOR 0x444444FF
# define UI_BUTTON_HOVER_COLOR 0x555555FF
# define UI_BUTTON_BORDER_COLOR 0x666666FF
# define UI_BUTTON_BORDER_HOVER_COLOR 0x888888FF
# define UI_TEXT_COLOR 0xFFFFFFFF
# define UI_SECTION_HEADER_COLOR 0x252538FF  // Darker blue-gray for section headers
# define UI_HEADER_COLOR 0x242437FF         // Darker blue-gray for main header
# define UI_BORDER_COLOR 0x3E3E5EFF         // Bluish border color
# define UI_TRANSPARENT 0x00000000


typedef enum e_ui_type
{
	UI_PANEL,       /**< Container element for grouping other elements */
	UI_BUTTON,      /**< Clickable button with text */
	UI_LABEL,       /**< Text label */
	UI_HEADER,      /**< Section header */
	UI_SECTION,     /**< Section container with header */
	UI_VALUE_BUTTON /**< Button that modifies a numeric value */
} t_ui_type;

typedef struct s_ui_images
{
	mlx_image_t	*button_img;
	mlx_image_t	*header_img;
	mlx_image_t	*panel_img;
} t_ui_images;


typedef struct s_ui_style
{
	uint32_t	bg_color;        /**< Background color in 0xRRGGBBAA format */
	uint32_t	border_color;    /**< Border color in 0xRRGGBBAA format */
	uint32_t	text_color;      /**< Text color in 0xRRGGBBAA format */
	float		padding;         /**< Padding in pixels */
	bool		visible;        /**< Visibility flag */
} t_ui_style;


typedef struct s_ui_label
{
	char		*text;
	uint32_t	color;
} t_ui_label;


typedef struct s_ui_element
{
	t_ui_type			type;           /**< Type of the UI element */
	t_v2f				pos;            /**< Position relative to parent */
	t_v2f				size;           /**< Size of the element */
	t_ui_style			style;          /**< Visual style properties */
	void				*data;          /**< Type-specific data */
	struct s_ui_element	*parent;        /**< Parent element (NULL for root) */
	struct s_ui_element	*first_child;   /**< First child element */
	struct s_ui_element	*next_sibling;  /**< Next sibling element */
	void			(*action)(struct s_ui_element *, void *); /**< Click action callback */
	void			(*render)(struct s_ui_element *, t_ui_context *); /**< Render function */
	bool			visible;        /**< Visibility flag */
	t_v2f			abs_pos;        /**< Absolute position in the window */
	int				state;          /**< Current interaction state */
} t_ui_element;


typedef struct s_ui_button
{
	void		(*on_click)(t_ui_element *, void *);
	void			*param;
	char			*label;
} t_ui_button;


typedef struct s_ui_value_button
{
	float			*value;
	float			step;
	t_v2f			range;
	t_ui_element	*value_label;
	void			(*on_click)(t_ui_element *, void *);
	void			*param;
	char			*label;
} t_ui_value_button;


typedef struct s_ui_context
{
	mlx_t		*mlx;           /**< MLX instance */
	t_scene		*scene;         /**< Reference to the scene data */
	mlx_image_t	*canvas;        /**< Main canvas image for UI rendering */
	int			canvas_instance; /**< Instance ID of the canvas */
	t_ui_images	*images;        /**< Cached images and textures */
	bool		is_visible;     /**< UI visibility flag */
	bool		needs_redraw;   /**< Flag indicating if UI needs redraw */
} t_ui_context;

typedef struct s_ui
{
	t_ui_element	*root;
	t_ui_context	*context;
} t_ui;


t_ui_context	*create_ui_context(mlx_t *mlx, t_scene *scene);
void		destroy_ui_context(t_ui_context *ctx);
t_ui		*create_ui(mlx_t *mlx, t_scene *scene);
void		destroy_ui(t_ui *ui);
void		render_ui(t_ui *ui);
void		toggle_ui_visibility(t_ui *ui);
bool		init_ui(t_game *game, t_scene *scene);
void		ui_element_setup_handlers(t_ui_element *element);
void		handle_ui_click(t_ui_element *root, int32_t x, int32_t y, t_ui_context *ctx);

t_ui_element	*ui_element_create(t_ui_type type, t_v2f pos, t_v2f size);
t_ui_element	*create_panel(t_ui_context *ctx, t_v2f pos, t_v2f size);
t_ui_element	*create_button(t_ui_context *ctx, const char *label, t_v2f pos, t_v2f size,
				void (*on_click)(t_ui_element *, void *), void *param);
t_ui_element	*create_label(t_ui_context *ctx, const char *text, t_v2f pos, uint32_t color);
t_ui_element	*create_value_button(t_ui_context *ctx, float *value,
							t_v2f range, float step, t_v2f pos, t_v2f size);
t_ui_element	*create_ambient_section(t_ui_context *ctx, t_scene *scene, t_v2f pos, t_v2f size);
t_ui_element	*create_header(t_ui_context *ctx, const char *title, t_v2f pos, t_v2f size);

void			destroy_ui_element(t_ui_element *element, t_ui_context *ctx, bool free_data);
void			attach_child(t_ui_element *parent, t_ui_element *child);
void			safe_call_destroy_handler(t_ui_element *element, t_ui_context *ctx);

void		ui_element_destroy(t_ui_element *element, t_ui_context *ctx, bool free_data);
void		destroy_ui_element_recursive(t_ui_element *element, t_ui_context *ctx, bool free_data);
bool		ui_element_remove_child(t_ui_element *parent, t_ui_element *child, 
			bool destroy, t_ui_context *ctx);
void		attach_child(t_ui_element *parent, t_ui_element *child);
void		update_button_value(t_ui_element *button, int32_t click_x, t_ui_context *ctx);
void		update_label_text(t_ui_element *label, const char *text, t_ui_context *ctx);
void		decrement_value_button(t_ui_element *btn, void *param);
void		increment_value_button(t_ui_element *btn, void *param);

void		default_button(t_ui_element *button, t_v2f pos, t_v2f size);
void		default_value_button(t_ui_element *button, t_v2f pos, t_v2f size, const char *label);
void		default_section(t_ui_element *section, t_v2f pos, t_v2f size);
void		default_header(t_ui_element *header, t_v2f pos, t_v2f size);
void		default_panel(t_ui_element *panel, t_v2f pos, t_v2f size);
void		default_label(t_ui_element *label, t_v2f pos, t_v2f size);

void		render_ui_element(t_ui_element *element, t_ui_context *ctx);
void		draw_button(t_ui_element *button, t_ui_context *ctx);
void		draw_char(mlx_image_t *img, char c, int x, int y, uint32_t color);
void		draw_text(mlx_image_t *img, const char *str, t_v2f pos, uint32_t color);
void		draw_rect(mlx_image_t *img, t_v2f pos, t_v2f size, uint32_t color);
void		draw_rect_border(mlx_image_t *img, t_v2f pos, t_v2f size, uint32_t color);

void		layout_vertical(t_ui_element *parent, float spacing);
void		layout_horizontal(t_ui_element *parent, float spacing);
#endif
