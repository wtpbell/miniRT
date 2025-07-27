/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:45:51 by bewong            #+#    #+#             */
/*   Updated: 2025/07/28 00:01:17 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


typedef struct s_ui t_ui;
typedef struct s_ui_element t_ui_element;
typedef void (*t_ui_destroy_func)(t_ui_element *element, mlx_t *mlx);
typedef void (*t_ui_render_func)(t_ui_element *, mlx_image_t *);
typedef void (*t_ui_click_func)(t_ui_element *element, int x, int y, void *param);
typedef void (*t_ui_update_func)(t_ui_element *, void *);


# define WIDTH 1600
# define HEIGHT 900
// UI Dimensions
# define UI_PANEL_WIDTH 250
# define UI_BUTTON_WIDTH 200
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
# define UI_BUTTON_BORDER_COLOR 0x666666FF
# define UI_BUTTON_BORDER_HOVER_COLOR 0x888888FF
# define UI_TEXT_COLOR 0xFFFFFFFF
# define UI_SECTION_HEADER_COLOR 0x222222FF
# define UI_HEADER_COLOR 0x333333FF
# define UI_BORDER_COLOR 0x666666FF
# define UI_TRANSPARENT 0x00000000

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
	struct s_ui_element	*parent;
	struct s_ui_element	*first_child;
	struct s_ui_element	*next_sibling;
	int32_t				instance_id;
	mlx_image_t			*image;
	void				*data;
	t_ui_render_func	render;
	t_ui_destroy_func	destroy;
	t_ui_update_func	update;
	void				(*action)(struct s_ui_element *, void *);
} t_ui_element;

typedef struct s_ui_button
{
	void		(*on_click)(t_ui_element *, void *);
	void		*param;
	char		*label;
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

typedef struct s_ui
{
	t_ui_element	*root;
	mlx_image_t		*ui_image;
	t_ui_images 	*images;
	bool			needs_redraw;
	mlx_t			*mlx;
} t_ui;

t_ui_images		*ui_images_create(mlx_t *mlx);
void			ui_images_destroy(mlx_t *mlx, t_ui_images *images);
t_ui_element	*ui_element_create(t_ui_type type, t_v2f pos, t_v2f size);
void			safe_call_destroy_handler(t_ui_element *element, mlx_t *mlx);
/* UI Core Functions */
t_ui			*create_ui(mlx_t *mlx, t_scene *scene);
void			destroy_ui(t_ui *ui);
bool			init_ui(t_game *game, t_scene *scene);
void			ui_element_setup_handlers(t_ui_element *element);
void			render_ui(t_ui *ui);
void			handle_ui_click(t_ui_element *root, int32_t x, int32_t y);

/* UI Element Creation */
t_ui_element	*create_ui_element(t_ui_type type, t_v2f pos, t_v2f size);
t_ui_element	*create_panel(mlx_t *mlx, t_v2f pos, t_v2f size);
t_ui_element	*create_label(mlx_t *mlx, const char *text, t_v2f pos, uint32_t color);
t_ui_element	*create_button(mlx_t *mlx, const char *label, t_v2f pos, t_v2f size,
				void (*on_click)(t_ui_element *, void *), void *param);
t_ui_element	*create_value_button(mlx_t *mlx, const char *label, float *value,
				t_v2f range, float step, t_v2f pos, t_v2f size);
t_ui_element	*create_section(mlx_t *mlx, const char *title, t_v2f pos, t_v2f size);
t_ui_element	*create_ambient_section(mlx_t *mlx, t_scene *scene, t_v2f pos, t_v2f size);
t_ui_element	*create_header(mlx_t *mlx, const char *title, t_v2f pos, t_v2f size);

/* UI Element Management */
void		ui_element_destroy(t_ui_element *element, mlx_t *mlx, bool free_data);
void		destroy_ui_element_recursive(t_ui_element *element, t_ui *ui);
bool		ui_element_remove_child(t_ui_element *parent, t_ui_element *child, 
				bool destroy, mlx_t *mlx);
void		attach_child(t_ui_element *parent, t_ui_element *child);
int32_t		ui_element_add_instance(mlx_t *mlx, t_ui_element *element);
void		ui_element_set_position(t_ui_element *element, int32_t x, int32_t y);
void		ui_element_set_visible(t_ui_element *element, bool visible);
bool		is_point_in_element(t_ui_element *element, int32_t x, int32_t y);
void		update_button_value(t_ui_element *button, int32_t click_x);

/* UI Default Styles */
void		default_button(t_ui_element *button, t_v2f pos, t_v2f size);
void		default_value_button(t_ui_element *button, t_v2f pos, t_v2f size, const char *label);
void		default_section(t_ui_element *section, t_v2f pos, t_v2f size);
void		default_header(t_ui_element *header, t_v2f pos, t_v2f size);
void		default_panel(t_ui_element *panel, t_v2f pos, t_v2f size);
void		default_label(t_ui_element *label, t_v2f pos, t_v2f size);

/* UI Rendering */
void		render_ui_element(t_ui_element *element, mlx_image_t *target);
void		draw_button(t_ui_element *button, mlx_image_t *target);
void		draw_char(mlx_image_t *img, char c, int x, int y, uint32_t color);
void		draw_text(mlx_image_t *img, const char *str, t_v2f pos, uint32_t color);
void		draw_rect(mlx_image_t *img, t_v2f pos, t_v2f size, uint32_t color);

#endif
