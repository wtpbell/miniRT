/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:45:51 by bewong            #+#    #+#             */
/*   Updated: 2025/08/09 16:04:30 by bewong           ###   ########.fr       */
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
# include "rt_types.h"
# include "parser.h"

# define UI_PANEL_WIDTH 300
# define UI_BUTTON_WIDTH 30
# define UI_BUTTON_HEIGHT 30

# define UI_LABEL_WIDTH_RATIO 0.3f
# define UI_BTN_WIDTH_RATIO 0.6f
# define UI_BTN_HEIGHT_RATIO 0.15f
# define UI_VERT_PADDING_RATIO 0.04f
# define UI_HORZ_PADDING_RATIO 0.05f

# define UI_HEADER_HEIGHT 30
# define UI_SECTION_HEADER_HEIGHT 30
# define UI_SECTION_PADDING 10
# define UI_PANEL_PADDING 0
# define UI_PADDING 4
# define UI_CHAR_WIDTH 8
# define UI_CHAR_HEIGHT 15
# define UI_FONT_HEIGHT 8
# define UI_ROW_HEIGHT 30
# define UI_LABEL_PADDING 10
# define UI_LABEL_HEIGHT 20


/* Base UI Colors */
# define UI_PANEL_BG_COLOR		0x1A1A1A77
# define UI_SECTION_COLOR		0x2D2D2DCC
# define UI_BUTTON_COLOR		0x4A90E2FF
# define UI_BUTTON_HOVER_COLOR	0x6AA8FFFF
# define UI_BUTTON_ACTIVE_COLOR	0x1A5FB4FF
# define UI_BUTTON_BORDER_COLOR	0xFFFFFFFF
# define UI_TEXT_COLOR			0xFFFFFFFF
# define UI_TEXT_SECONDARY_COLOR	0xAAAAAAFF
# define UI_SECTION_HEADER_COLOR	0x2D2D2DFF
# define UI_HEADER_COLOR		0x1A1A1AFF
# define UI_BORDER_COLOR		0x4A4A4AFF
# define UI_TRANSPARENT			0x00000000
# define UI_LABEL_COLOR			0xFFFFFFFF

/* Section Colors */
# define UI_COLOR_CAMERA_SECTION	0xE74C3CFF
# define UI_COLOR_LIGHT_SECTION		0x9B59B6FF
# define UI_COLOR_AMBIENT_SECTION	0xF1C40FFF
# define UI_COLOR_DOF_SECTION		0xd14d25FF
# define UI_COLOR_SAMPLE_SECTION	0xD125ACFF 

// /* Special Elements */
# define UI_RENDER_BUTTON_COLOR		0x2ECC71FF

t_ui_element	*create_ambient_section(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);
t_ui_element	*create_camera_section(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);
t_ui_element	*create_light_section(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);
t_ui_element	*create_ui_sections(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);
t_ui_element	*create_dof_section(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);
t_ui_element	*create_sample_section(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);

typedef enum e_ui_type
{
	UI_PANEL,
	UI_BUTTON,
	UI_LABEL,
	UI_HEADER,
	UI_SECTION,
	UI_VALUE_BUTTON
}	t_ui_type;

typedef struct s_ui_images
{
	mlx_image_t	*button_img;
	mlx_image_t	*header_img;
	mlx_image_t	*panel_img;
}	t_ui_images;

typedef struct s_ui_style
{
	uint32_t	bg_color;
	uint32_t	border_color;
	uint32_t	text_color;
	float		padding;
	bool		visible;
}	t_ui_style;

typedef struct s_ui_label
{
	char		*text;
	uint32_t	color;
}	t_ui_label;

typedef struct s_ui_element
{
	t_ui_type			type;
	t_v2f				pos;
	t_v2f				size;
	t_ui_style			style;
	void				*data;
	struct s_ui_element	*parent;
	struct s_ui_element	*first_child;
	struct s_ui_element	*next_sibling;
	void				(*action)(struct s_ui_element *, void *);
	bool				visible;
	t_v2f				abs_pos;
}	t_ui_element;

typedef struct s_ui_btn
{
	void				(*on_click)(t_ui_element *, void *);
	void				*param;
	char				*label;
}	t_ui_btn;

typedef struct s_ui_vbtn
{
	float				*value;
	float				step;
	t_v2f				range;
	t_ui_element		*value_label;
	void				(*on_click)(t_ui_element *, void *);
	void				*param;
	char				*label;
	char				*(*formatter)(float);
}	t_ui_vbtn;

typedef struct s_ui_context
{
	mlx_t				*mlx;
	t_scene				*scene;
	mlx_image_t			*canvas;
	int					canvas_instance;
	t_ui_images			*images;
	bool				is_visible;
	bool				needs_redraw;
	void				*game;
}	t_ui_context;

typedef struct s_ui
{
	t_ui_element	*root;
	t_ui_context	*context;
}	t_ui;

typedef struct s_vbtn_config
{
	t_ui_context	*ctx;
	float			*value;
	t_v2f			range;
	float			step;
	t_v2f			pos;
	t_v2f			size;
	char			*(*formatter)(float);
}	t_vbtn_config;

struct s_ui_sections
{
	float			height_scale;
	t_ui_element	*(*create_func)(t_ui_context*, t_sample*, t_v2f, t_v2f);
};

extern struct s_ui_sections g_sections[];

/* UI Context Management */
t_ui_context	*create_ui_context(mlx_t *mlx, t_scene *scene, void *game_ptr);
void			destroy_ui_context(t_ui_context *ctx);
t_ui			*create_ui(mlx_t *mlx, t_scene *scene, t_sample *sample, void *game_ptr);
void			destroy_ui(t_ui *ui);
void			render_ui(t_ui *ui);
void			toggle_ui_visibility(t_ui *ui);

/* UI Element Creation */
t_ui_element	*create_ui_element(t_ui_type type, t_v2f pos, t_v2f size);
t_ui_element	*create_header(t_ui_context *ctx, const char *title, t_v2f pos, t_v2f size);
t_ui_element	*create_label(t_ui_context *ctx, const char *text, t_v2f pos, uint32_t color);
t_ui_element	*create_labeled_control(t_vbtn_config *cfg, const char *label_text, float total_width);
t_ui_element	*create_panel(t_ui_context *ctx, t_v2f pos, t_v2f size);
t_ui_element	*create_button(t_ui_context *ctx, const char *label_text, t_v2f pos, t_v2f size,
					void (*on_click)(t_ui_element *, void *), void *param);
t_ui_element	*create_ambient_section(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);
t_ui_element	*create_camera_section(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);
t_ui_element	*create_light_section(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);
t_ui_element	*create_ui_sections(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);
t_ui_element	*create_dof_section(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);
t_ui_element	*create_sample_section(t_ui_context *ctx, t_sample *sample, t_v2f pos, t_v2f size);

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
void			init_value_button_data(t_ui_vbtn *value_btn, const t_vbtn_config *cfg);
void			add_inc_dec_buttons(t_ui_element *container, const t_vbtn_config *cfg, t_ui_vbtn *value_btn);
void			add_value_label(t_ui_element *container, const t_vbtn_config *cfg, t_ui_vbtn *value_btn);

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
