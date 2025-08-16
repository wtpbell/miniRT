vpath %.c src:src/parser/core:src/parser/objects:src/parser/elements:src/parser/utils:src/math:src/math/vector:src/container:src/math:src/math/vector:src/render:src/render:src/ui:src/math/noise:src/rt_snprintf:src/perlin_display

NAME		:= miniRT
CC			:= cc
C_FLAGS		:= -Werror -Wall -Wextra -O3 -fsanitize=address,undefined
C_LINK		:= -ldl -lglfw -pthread -lm -flto

BIN_DIR		:= bin/
LIB_DIR		:= lib/
MLX42_DIR	:= $(LIB_DIR)mlx42
LIBFT_DIR	:= $(LIB_DIR)libft

MLX42		:= $(addprefix $(MLX42_DIR)/build/, libmlx42.a)
LIBFT		:= $(addprefix $(LIBFT_DIR)/, libft.a)

INC			:= -I ./include -I $(MLX42_DIR)/include -I $(LIBFT_DIR)/include

PARSER_CORE	:= parser.c element_parser.c camera.c light.c sphere.c plane.c\
				cylinder.c cone.c string_utils.c vector_utils.c error.c cleanup.c\
				string_to_num.c general_utils.c validate_utils.c triangle.c\
				field.c material.c texture.c texture_utils.c
SRCS_MAIN  := main.c vector_init.c vector_helper.c vector_operations.c\
				vec_container.c vec_container_utils.c color.c render.c\
				rt_math.c range.c matrix.c game.c rt_sphere.c rt_plane.c rt_cylinder.c\
				color_utils.c random_utils.c rt_light.c rt_dof.c \
				quit.c rt_triangle.c rt_material.c material_init.c light_utils.c\
				material_utils.c obj_utils.c procedural_texturing.c rt_cone.c\
				perlin.c random_vector.c matrix_utils.c bump_map.c rt_texture.c v2f.c\
				vector_core.c matrix_space.c rt_material_utils.c rt_triangle_uv.c\
				rt_cylinder_uv.c ui_core.c ui_layout.c ui_render.c ui_default.c\
				ui_utils.c ui_element_handlers.c ui_render_loop.c \
				threads.c benchmark.c ui_ambient_section.c ui_light_section.c \
				ui_camera_section.c ui_dof_section.c ui_sample_section.c \
				ui_cleanup.c ui_layout_utils.c ui_draw.c ui_event_button.c \
				ui_event_click.c ui_element_basic.c ui_element_creation.c \
				noise_texturing.c lerp.c v2f_helper.c perlin_display.c
SNPRINTF	:= rt_snprintf.c rt_snprintf_str.c rt_snprintf_num.c
SRCS_DEBUG	:= print_var.c
SRCS		:= $(SRCS_MAIN) $(SRCS_DEBUG) $(PARSER_CORE) $(SNPRINTF)
OBJS 		:= $(SRCS:%.c=$(BIN_DIR)%.o)

all: $(LIBFT) $(MLX42) $(NAME)

debug: C_FLAGS += -g3 -fsanitize=address,undefined
debug: all
	@echo 'use: LSAN_OPTIONS="suppressions=fsan_supp.supp" ./miniRT asset/<scene>.rt'
	@echo 'use: TSAN_OPTIONS="suppressions=tsan_supp.supp" ./miniRT asset/<scene>.rt'

val: C_FLAGS += -g3
val: clean all
	@valgrind --leak-check=full --track-origins=yes --suppressions=mlx42.supp ./$(NAME) $(ARG)

bonus: all

$(NAME): $(BIN_DIR) $(OBJS)
	@$(CC) $(C_FLAGS) $(OBJS) $(LIBFT) $(MLX42) $(C_LINK) $(INC) -o $(NAME)
	@echo Build complete!

$(BIN_DIR)%.o: %.c
	@$(CC) $(C_FLAGS) -c $< -o $@ $(INC)

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

.PHONY: all clean fclean re debug val
