/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ui.h                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/23 13:45:51 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/22 12:27:58 by jboon         ########   odam.nl         */
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
# define UI_HEADER_HEIGHT 30
# define UI_PADDING 4
# define UI_CHAR_WIDTH 8
# define UI_CHAR_HEIGHT 15
# define UI_FONT_HEIGHT 8
# define UI_ROW_HEIGHT 30
# define UI_LABEL_PADDING 10

/* Base UI Colors */
# define UI_PANEL_BG_COLOR			0x1A1F2CFF
# define UI_SECTION_COLOR			0x2D2D2DCC
# define UI_BUTTON_COLOR			0xEBAB15D2
# define UI_BUTTON_BORDER_COLOR		0xFFFFFFFF
# define UI_TEXT_COLOR				0xFFFFFFFF
# define UI_HEADER_COLOR			0x1A1A1AFF
# define UI_BORDER_COLOR			0x4A4A4AFF
# define UI_TRANSPARENT				0xFFFFFF00
# define UI_LABEL_COLOR				0xFFFFFFCF
# define UI_RENDER_BUTTON_COLOR		0x60EA9AFF

/* Section Colors */
# define UI_COLOR_CAMERA_SECTION	0xD12525FF
# define UI_COLOR_LIGHT_SECTION		0xA303CFFF
# define UI_COLOR_AMBIENT_SECTION	0x2F08CFFF
# define UI_COLOR_DOF_SECTION		0x046D00FF
# define UI_COLOR_SAMPLE_SECTION	0x2F6BA4FF

extern const uint8_t		g_font[256][8];

typedef enum e_ui_type
{
	UI_PANEL,
	UI_BUTTON,
	UI_LABEL,
	UI_HEADER,
	UI_SECTION,
	UI_VALUE_BUTTON
}	t_ui_type;

typedef struct s_ui_style
{
	uint32_t	bg_color;
	uint32_t	border_color;
	uint32_t	text_color;
}	t_ui_style;

typedef struct s_ui_label
{
	char		*text;
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
	bool				is_visible;
	bool				needs_redraw;
	void				*game;
	bool				is_dirty;
}	t_ui_context;

struct s_ui
{
	t_ui_element	*root;
	t_ui_context	*context;
};

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

typedef struct s_btn_config
{
	t_ui_context	*ctx;
	const char		*label_text;
	t_v2f			pos;
	t_v2f			size;
	void			(*on_click)(t_ui_element *, void *);
	void			*param;
}	t_btn_config;

typedef struct s_section_config
{
	t_ui_context	*ctx;
	t_sample		*sample;
	t_v2f			pos;
	t_v2f			size;
}	t_section_config;

struct s_ui_sections
{
	float			height_scale;
	t_ui_element	*(*create_func)(t_section_config*);
};

typedef struct s_sprite
{
	mlx_image_t	*img;
	t_v2f		anchor;
	t_v2f		scale;
	t_v2i		full_size;
}	t_sprite;

typedef struct s_animation
{
	t_v2i		pos;
	uint32_t	idx;
	uint32_t	frame_count;
	float		fps;
	float		time;
	t_sprite	*frames;
}	t_ani;

typedef struct s_progress_bar
{
	t_v2i		pos;
	t_sprite	text;
	t_sprite	bg;
	uint32_t	bg_color;
	uint32_t	bar_color;
	t_v2i		size;
}	t_progress_bar;

typedef struct s_load_screen
{
	t_sprite		bg;
	t_ani			ani;
	t_progress_bar	pb;
	t_v2i			ren_prog;
}	t_load_screen;

extern struct s_ui_sections	g_sections[];

/* UI Context Management */
void			destroy_ui_context(t_ui_context *ctx);
t_ui			*create_ui(mlx_t *mlx, t_scene *scene,
					t_sample *sample, void *game_ptr);
void			destroy_ui(t_ui *ui);
void			render_ui(t_ui *ui);
void			toggle_ui_visibility(t_ui *ui);

/* UI Element Creation */
t_ui_element	*create_header(t_ui_context *ctx, const char *title,
					t_v2f pos, t_v2f size);
t_ui_element	*create_label(t_ui_context *ctx, const char *text,
					t_v2f pos, uint32_t color);
t_ui_element	*create_labeled_control(t_vbtn_config *cfg,
					const char *label_text, float total_width);
t_ui_element	*create_panel(t_ui_context *ctx, t_v2f pos, t_v2f size);
t_ui_element	*create_button(t_btn_config *cfg);
t_ui_element	*create_ambient_section(t_section_config *cfg);
t_ui_element	*create_camera_section(t_section_config *cfg);
t_ui_element	*create_light_section(t_section_config *cfg);
t_ui_element	*create_ui_sections(t_section_config *cfg);
t_ui_element	*create_dof_section(t_section_config *cfg);
t_ui_element	*create_sample_section(t_section_config *cfg);

/* UI Element Management */
void			destroy_ui_element(t_ui_element *element);
void			attach_child(t_ui_element *parent, t_ui_element *child);
void			update_button_value(t_ui_element *button, int32_t click_x,
					t_ui_context *ctx);
void			handle_ui_click(t_ui_element *root, int32_t x, int32_t y,
					t_ui_context *ctx);

/* UI Rendering */
void			render_loop(void *param);
void			draw_button(t_ui_element *button, t_ui_context *ctx);
void			draw_text(mlx_image_t *img, const char *str,
					t_v2f pos, uint32_t color);
void			draw_rect(mlx_image_t *img, t_v2f pos, t_v2f size,
					uint32_t color);
void			draw_rect_border(mlx_image_t *img, t_v2f pos, t_v2f size,
					uint32_t color);
void			ui_mark_dirty(t_ui_context *ctx);

/* UI Helpers */
char			*format_float_value(float value);
char			*format_color_value(float value);
t_light			*find_light(t_scene *scene, t_light_type type);
void			decrement_value_button(t_ui_element *btn, void *param);
void			increment_value_button(t_ui_element *btn, void *param);
void			update_value_label(t_ui_vbtn *btn, t_ui_context *ctx);
void			init_value_button_data(t_ui_vbtn *value_btn,
					const t_vbtn_config *cfg);
void			add_inc_dec_buttons(t_ui_element *container,
					const t_vbtn_config *cfg);
void			add_value_label(t_ui_element *container,
					const t_vbtn_config *cfg, t_ui_vbtn *value_btn);
void			render_button_clicked(t_ui_element *button, void *param);

/* Default Styling */
void			default_button(t_ui_element *button, t_v2f pos, t_v2f size);
void			default_section(t_ui_element *section, t_v2f pos, t_v2f size);
void			default_header(t_ui_element *header, t_v2f pos, t_v2f size);
void			default_panel(t_ui_element *panel, t_v2f pos, t_v2f size);
void			default_label(t_ui_element *label, t_v2f pos, t_v2f size);

/* Utility */
uint32_t		blend_colors(uint32_t bg, uint32_t fg);

/* Load Screen */
t_load_screen	*init_load_screen(mlx_t *mlx);
void			destroy_load_screen(t_load_screen *load_screen, mlx_t *mlx);
void			update_load_screen(t_load_screen *screen, float delta,
					float progress);

/* Sprite Drawing and Animation */
void			update_animation(t_ani *ani, t_sprite *bg, float delta);
// void			draw_frame(mlx_image_t *dst, mlx_image_t *src, t_v2i draw_pos);
void			draw_frame(t_sprite *dst, t_sprite *src, t_v2i draw_pos);
mlx_image_t		*init_str_frame(mlx_t *mlx, const char *str);
t_v2i			get_sprite_position(t_sprite *parent, t_sprite *child,
					t_v2i pos);
void			init_sprite_full_size(t_sprite *sprite);

/* Progress Bar */
bool			init_progress_bar(t_progress_bar *pb, t_v2i pos, t_v2i size,
					mlx_t *mlx);
void			update_progress_bar(t_progress_bar *pb, t_sprite *bg, float t);

#endif
