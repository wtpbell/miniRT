/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:45:51 by bewong            #+#    #+#             */
/*   Updated: 2025/07/25 00:51:07 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file ui.h
 * @brief User Interface module for miniRT
 * 
 * This module provides a flexible UI system built on top of MLX42, supporting
 * various UI elements like buttons, labels, panels, and sections. The UI system
 * uses an instance-based rendering approach for better performance.
 * 
 * The UI hierarchy is organized as a tree of t_ui_element nodes, where each
 * element can have children, allowing for complex layouts. The system supports
 * event handling, styling, and automatic layout management.
 * 
 * Key Features:
 * - Instance-based rendering for better performance
 * - Hierarchical UI element structure
 * - Flexible styling system
 * - Event handling for user interactions
 * - Automatic layout management
 */

#ifndef UI_H
# define UI_H

# include "minirt.h"
# include "scene.h"
# include <stdbool.h>
# include <stdint.h>
# include <stddef.h>
# include "MLX42/MLX42.h"

// UI Dimensions
# define UI_PANEL_WIDTH 250
# define UI_BUTTON_WIDTH 200
# define UI_BUTTON_HEIGHT 30
# define UI_BUTTON_SPACING 10
# define UI_HEADER_HEIGHT 30
# define UI_HEADER_WIDTH 230
# define UI_SECTION_HEADER_HEIGHT 30
# define UI_SECTION_PADDING 10
# define UI_SECTION_SPACING 10
# define UI_CHAR_WIDTH 8
# define UI_CHAR_HEIGHT 15

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
# define UI_PANEL_BG_COLOR 0x111111FF
# define UI_SECTION_COLOR 0x222222FF
# define UI_BUTTON_COLOR 0x444444FF
# define UI_BUTTON_HOVER_COLOR 0x555555FF
# define UI_TEXT_COLOR 0xFFFFFFFF
# define UI_SECTION_HEADER_COLOR 0x222222FF
# define UI_HEADER_COLOR 0x333333FF
# define UI_BORDER_COLOR 0xFFFFFFFF


typedef enum e_ui_type
{
	UI_PANEL,
	UI_BUTTON, 
	UI_LABEL,
	UI_HEADER,
	UI_SECTION,
	UI_VALUE_BUTTON
} t_ui_type;

typedef struct s_ui_images
{
	mlx_image_t	*button_img;
	mlx_image_t	*header_img;
	mlx_image_t	*panel_img;
} t_ui_images;


typedef struct s_ui_style
{
	uint32_t	bg_color;
	uint32_t	fg_color;
	uint32_t	text_color;
	uint32_t	border_color;
	int			padding;
	bool		visible;
} t_ui_style;


typedef struct s_ui_label
{
	char		*text;
	uint32_t	color;
} t_ui_label;


typedef struct s_ui_element
{
	t_ui_type			type;
	t_ui_style			style;
	t_v2f				pos;
	t_v2f				size;
	bool				visible;
	bool				hovered;
	bool				pressed;
	struct s_ui_element	*parent;
	struct s_ui_element	*first_child;
	struct s_ui_element	*next_sibling;
	int32_t				instance_id;
	mlx_image_t			*image;
	void				*data;
	void				(*action)(struct s_ui_element *, void *);
} t_ui_element;


typedef struct s_ui_button
{
	char		*label_text;
	float		*value;
	float		step;
	t_v2f		range;
	void		*param;
	bool		is_hovered;
	int			instance_id;
	mlx_image_t	*img;
	void		(*on_click)(t_ui_element *, void *);
} t_ui_button;

typedef struct s_ui
{
	t_ui_element	*root;
	mlx_image_t		*ui_image;
	t_ui_images 	*images;
	bool			needs_redraw;
	mlx_t			*mlx;
} t_ui;


typedef struct s_game
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_ui		*ui;
	t_scene		*scene;
} t_game;


t_ui_images		*ui_images_create(mlx_t *mlx);
void			ui_images_destroy(mlx_t *mlx, t_ui_images *images);
t_ui_element	*ui_element_create_with_image(mlx_t *mlx, t_ui_type type, t_v2f pos, t_v2f size);
void			ui_element_render(t_ui_element *element);
int32_t			ui_element_add_instance(mlx_t *mlx, t_ui_element *element);
void			ui_element_set_position(t_ui_element *element, int32_t x, int32_t y);
void			ui_element_set_visible(t_ui_element *element, bool visible);

bool			init_ui(t_game *game, t_scene *scene);
t_ui			*ui_create(mlx_t *mlx, t_scene *scene);
void			ui_add_child(t_ui_element *parent, t_ui_element *child);


t_ui_element	*create_panel(mlx_t *mlx, t_v2f pos, t_v2f size);
t_ui_element	*create_slider(mlx_t *mlx, const char *label, t_v2f pos, t_v2f size,
							float *value, float min, float max, float step);
t_ui_element	*create_color_control(mlx_t *mlx, const char *label, t_v2f pos, t_v2f size,
							t_v3f *color);
t_ui_element	*create_label(mlx_t *mlx, const char *text, t_v2f pos, uint32_t color);
t_ui_element	*create_button(mlx_t *mlx, const char *label, t_v2f pos, t_v2f size, 
							 void (*on_click)(t_ui_element *, void *), void *param);
t_ui_element	*create_value_button(mlx_t *mlx, const char *label, float *value, 
								   t_v2f range, float step, t_v2f pos, t_v2f size);
t_ui_element	*create_section(mlx_t *mlx, const char *title, t_v2f pos, t_v2f size);
t_ui_element	*create_ambient_section(mlx_t *mlx, t_scene *scene, t_v2f pos, t_v2f size);


void	ui_set_button_action(t_ui_element *button, void (*action)(t_ui_element *, void *), void *param);
void	ui_set_label_text(t_ui_element *label, const char *text);
void	ui_set_style(t_ui_element *element, t_ui_style style);
void	ui_set_visible(t_ui_element *element, bool visible);
void	ui_set_position(t_ui_element *element, t_v2f pos);
void	ui_element_destroy(t_ui_element *element);
void	render_ui(t_ui *ui);
void	ui_mouse_move(t_ui *ui, int32_t x, int32_t y);
void	ui_mouse_click(t_ui *ui, int32_t x, int32_t y, int32_t button);
void	ui_mouse_release(t_ui *ui, int32_t x, int32_t y, int32_t button);
t_ui	*create_ui(mlx_t *mlx, t_scene *scene);
void	destroy_ui(t_ui *ui);
void	render_ui(t_ui *ui);
void	render_ui_element(t_ui_element *element, mlx_image_t *target);
void	draw_rect(mlx_image_t *img, t_v2f pos, t_v2f size, uint32_t color);
void	draw_text(mlx_image_t *img, const char *text, t_v2f pos, uint32_t color);
void	draw_char(mlx_image_t *img, char c, int x, int y, uint32_t color);
void	handle_ui_click(t_ui_element *root, int32_t x, int32_t y);
bool	is_point_in_element(t_ui_element *element, int32_t x, int32_t y);
void	update_button_value(t_ui_element *button, int32_t click_x);
int 	game_loop(void *param);
void	destroy_ui_element_recursive(t_ui_element *element, t_ui *ui);

#endif
