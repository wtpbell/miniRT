vpath %.c src
vpath %.c src/bvh
vpath %.c src/container
vpath %.c src/math:src/math/noise:src/math/vector
vpath %.c src/parser:src/parser/core:src/parser/elements:src/parser/obj:src/parser/objects:src/parser/utils
vpath %.c src/perlin_display
vpath %.c src/render
vpath %.c src/rt_snprintf
vpath %.c src/threads
vpath %.c src/ui

NAME		:= miniRT
CC			:= cc
C_FLAGS		:= -Werror -Wall -Wextra -O3
C_LINK		:= -ldl -lglfw -pthread -lm -flto
D_FLAG		:= -D RT_HIGH

BIN_DIR		:= bin/
LIB_DIR		:= lib/
MLX42_DIR	:= $(LIB_DIR)mlx42
LIBFT_DIR	:= $(LIB_DIR)libft

MLX42		:= $(addprefix $(MLX42_DIR)/build/, libmlx42.a)
LIBFT		:= $(addprefix $(LIBFT_DIR)/, libft.a)

INC			:= -I ./include -I $(MLX42_DIR)/include -I $(LIBFT_DIR)/include

SRCS_BVH	:= bvh.c bvh_utils.c

SRCS_VEC	:= vec_container.c vec_container_utils.c

SRCS_MATH	:= vector_core.c vector_helper.c vector_init.c v2f.c v2f_helper.c\
				vector_operations.c matrix_utils.c random_vector.c rt_math.c\
				range.c random_utils.c matrix_space.c lerp.c perlin.c matrix.c

PARSER_CORE	:= material_utils.c string_to_num.c error.c field.c general_utils.c\
				string_utils.c vector_utils.c object_utils.c validate_utils.c\
				cleanup.c texture_utils.c plane.c sphere.c mesh.c triangle.c\
				cylinder.c cone.c obj.c obj_utils.c construct_mesh.c\
				parse_face.c parse_vertex.c element_parser.c parser.c\
				rt_strtok.c texture.c material.c light.c camera.c

SRCS_PRL_UI	:= perlin_param_control.c perlin_key_hook.c perlin_utils.c\
				perlin_param_update.c perlin_display.c perlin_modifier.c\
				perlin_math.c perlin_init.c

SRCS_RENDER	:= aabb.c bump_map.c color.c color_utils.c light_utils.c\
				material_init.c noise_texturing.c procedural_texturing.c\
				render.c rt_cone.c rt_cylinder.c rt_cylinder_uv.c rt_dof.c\
				rt_light.c rt_material.c rt_material_utils.c rt_mesh.c\
				rt_plane.c rt_sphere.c rt_texture.c rt_triangle.c\
				rt_triangle_uv.c 

SRCS_UI		:= ui_sample_section.c ui_draw.c ui_loading_screen.c ui_math.c\
				ui_update_animation.c ui_event_button.c ui_dof_section.c\
				ui_element_handlers.c ui_default.c ui_layout.c\
				ui_camera_section.c ui_cleanup.c ui_progress_bar.c\
				ui_draw_frame.c ui_light_section.c ui_event_click.c\
				ui_layout_utils.c ui_element_basic.c ui_core.c ui_utils.c\
				ui_ambient_section.c ui_render_loop.c ui_element_creation.c\
				ui_create.c

SNPRINTF	:= rt_snprintf.c rt_snprintf_str.c rt_snprintf_num.c rt_snprintf_real.c

SRCS_THRD	:= thread_data.c threads.c

SRCS_MAIN  := game.c scene_cleanup.c rt_exit.c benchmark.c game_hooks.c main.c

SRCS		:= $(SRCS_MAIN) $(PARSER_CORE) $(SNPRINTF) $(SRCS_BVH) $(SRCS_VEC)\
				$(SRCS_MATH) $(SRCS_PRL_UI) $(SRCS_RENDER) $(SRCS_UI) $(SRCS_THRD)
OBJS 		:= $(SRCS:%.c=$(BIN_DIR)%.o)

all: $(LIBFT) $(MLX42) $(NAME)

low: D_FLAG :=
low: all

debug: C_FLAGS += -g3 -fsanitize=address,undefined
debug: all
	@echo 'use: LSAN_OPTIONS="suppressions=fsan_supp.supp" ./miniRT asset/<scene>.rt'
	@echo 'use: TSAN_OPTIONS="suppressions=tsan_supp.supp" ./miniRT asset/<scene>.rt'

val: C_FLAGS += -g3
val: all
	@valgrind --leak-check=full --track-origins=yes --suppressions=mlx42.supp -q ./$(NAME) $(ARG)

bonus: all

$(NAME): $(BIN_DIR) $(OBJS)
	@$(CC) $(C_FLAGS) $(OBJS) $(LIBFT) $(MLX42) $(C_LINK) $(INC) -o $(NAME)
	@echo Build complete!

$(BIN_DIR)%.o: %.c
	@$(CC) $(C_FLAGS) $(D_FLAG) -c $< -o $@ $(INC)

$(LIBFT): $(LIBFT_DIR)
	@make -C $(LIBFT_DIR) bonus

$(MLX42): $(MLX42_DIR)
	@cmake -S $(MLX42_DIR) -B $(MLX42_DIR)/build && make -C $(MLX42_DIR)/build -j4

$(LIBFT_DIR):
	@make $(LIB_DIR)
	@git clone git@github.com:JoseJBoon/libft.git $(LIBFT_DIR)

$(MLX42_DIR):
	@make $(LIB_DIR)
	@git clone --branch v2.4.1 https://github.com/codam-coding-college/MLX42.git $(MLX42_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

clean:
	@make -C $(LIBFT_DIR) clean
	@$(RM) $(OBJS)
	@$(RM) $(NAME)
	@echo "Clean complete!"

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@$(RM) -r $(MLX42_DIR)/build
	@$(RM) -r $(BIN_DIR)
	@echo "Fclean complete!"

re: fclean all

.PHONY: all clean fclean re debug val low
