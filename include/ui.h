/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:45:51 by bewong            #+#    #+#             */
/*   Updated: 2025/07/23 13:45:51 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# include "minirt.h"
# include "scene.h"
# include <stdbool.h>
# include <stdint.h>
# include <stddef.h>
# include "MLX42/MLX42.h"

// t_ui_element (base)
// ├── type (UI_BUTTON, UI_LABEL, UI_SECTION, UI_HEADER, UI_PANEL)
// ├── style (t_ui_style)
// │   ├── bg_color
// │   ├── fg_color
// │   ├── text_color
// │   ├── border_color
// │   ├── padding
// │   └── visible
// ├── pos (t_v2f)
// ├── size (t_v2f)
// ├── *parent
// ├── *first_child
// ├── *next_sibling
// ├── *data (can point to specific UI element data like t_ui_button or t_ui_label)
// └── *action (function pointer for click events)

# define UI_PANEL_WIDTH 250
# define UI_BUTTON_WIDTH 200
# define UI_BUTTON_HEIGHT 30
# define UI_BUTTON_SPACING 20
# define UI_SECTION_HEADER_HEIGHT 30
# define UI_SECTION_WIDTH 250
# define UI_CHAR_WIDTH 8
# define UI_CHAR_HEIGHT 15
# define UI_BG_COLOR 0x11111180
# define UI_PANEL_BG_COLOR 0x111111FF
# define UI_SECTION_COLOR 0x222222FF
# define UI_SECTION_HEADER_COLOR 0x222222FF
# define UI_HEADER_COLOR 0x333333FF
# define UI_TEXT_COLOR 0xFFFFFFFF
# define UI_BUTTON_COLOR 0x444444FF
# define UI_BUTTON_HOVER_COLOR 0x666666FF
# define UI_BUTTON_ACTIVE_COLOR 0x888888FF
# define UI_BORDER_COLOR 0xFFFFFFFF

typedef enum e_ui_type
{
	UI_PANEL,
	UI_SECTION,
	UI_HEADER,
	UI_BUTTON,
	UI_LABEL
}	t_ui_type;

typedef struct s_ui_style
{
	uint32_t	bg_color;
	uint32_t	fg_color;
	uint32_t	text_color;
	uint32_t	border_color;
	int			padding;
	bool		visible;
}	t_ui_style;

typedef struct s_ui_element
{
	t_ui_type			type;
	t_ui_style			style;
	t_v2f				pos;
	t_v2f				size;
	struct s_ui_element	*parent;
	struct s_ui_element	*first_child;
	struct s_ui_element	*next_sibling;
	void				*data;
	void				(*action)(struct s_ui_element *, void *);
}	t_ui_element;

typedef struct s_ui
{
	mlx_image_t		*ui_layer;
	t_ui_element	*root;
	mlx_t			*mlx;
	bool			needs_redraw;
}	t_ui;

typedef struct s_game
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_ui		*ui;
	t_scene		*scene;
}	t_game;

typedef struct s_ui_button
{
	mlx_image_t	*img;
	char		*label_text;
	float		*value;
	float		step;
	int			instance_id;
	t_v2f		range;
}	t_ui_button;

typedef struct s_ui_label
{
	char		*text;
	uint32_t	color;
}	t_ui_label;


bool			init_ui(t_game *game, t_scene *scene);
void			cleanup_ui(t_ui *ui);
t_ui			*ui_create(mlx_t *mlx, t_scene *scene);
void			ui_destroy(t_ui *ui);
t_ui			*get_ui_instance(void);
t_ui_element	*ui_element_create(t_ui_type type, t_v2f pos, t_v2f size);
void			ui_add_child(t_ui_element *parent, t_ui_element *child);
t_ui_element	*create_panel(t_v2f pos, t_v2f size);
t_ui_element	*create_section(const char *title, t_v2f pos, t_v2f size);
t_ui_element	*create_header(const char *title, t_v2f pos, t_v2f size);
t_ui_element	*create_label(const char *text, t_v2f pos, uint32_t color);
t_ui_element	*create_value_button(const char *label, float *value,
								t_v2f range, float step);
t_ui_element	*create_ambient_section(t_scene *scene, t_v2f pos);
void			render_ui(t_ui *ui);
void			render_ui_element(t_ui_element *element, mlx_image_t *target);
void			draw_rect(mlx_image_t *img, t_v2f pos, t_v2f size, uint32_t color);
void			draw_text(mlx_image_t *img, const char *text, t_v2f pos, uint32_t color);
void			draw_char(mlx_image_t *img, char c, int x, int y, uint32_t color);
void			handle_ui_click(t_ui_element *root, int32_t x, int32_t y);
bool			is_point_in_element(t_ui_element *element, int32_t x, int32_t y);
void			update_button_value(t_ui_element *button, int32_t click_x);
int				game_loop(void *param);
void			destroy_ui(t_ui *ui);

#endif
