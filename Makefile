vpath %.c src:src/parser/core:src/parser/objects:src/parser/elements:src/parser/utils:src/math:src/math/vector:src/container:src/render

NAME		:= miniRT
CC			:= cc
C_FLAGS		:= -Werror -Wall -Wextra -Ofast
C_LINK		:= -ldl -lglfw -pthread -lm

BIN_DIR		:= bin/
LIB_DIR		:= lib/
MLX42_DIR	:= $(LIB_DIR)mlx42
LIBFT_DIR	:= $(LIB_DIR)libft

MLX42		:= $(addprefix $(MLX42_DIR)/build/, libmlx42.a)
LIBFT		:= $(addprefix $(LIBFT_DIR)/, libft.a)

INC			:= -I ./include -I $(MLX42_DIR)/include -I $(LIBFT_DIR)/include

PARSER_CORE	:= parser.c element_parser.c file_parser.c camera.c light.c\
				sphere.c plane.c cylinder.c string_utils.c vector_utils.c\
				error.c cleanup.c string_to_num.c token_utils.c\
				general_utils.c validate_utils.c
SRCS_MAIN	:= main.c vector_init.c vector_helper.c vector_operation.c\
				vec_container.c vec_container_utils.c color.c render.c\
				rt_math.c matrix.c game.c rt_sphere.c rt_plane.c rt_cylinder.c \
				color_utils.c material.c random_utils.c rt_light.c
SRCS_DEBUG	:= print_var.c
SRCS		:= $(SRCS_MAIN) $(SRCS_DEBUG) $(PARSER_CORE)
OBJS 		:= $(SRCS:%.c=$(BIN_DIR)%.o)

all: $(LIBFT) $(MLX42) $(NAME)

debug: C_FLAGS += -g3 -fsanitize=address,undefined
debug: all

val: C_FLAGS += -g3
val: clean all
	@valgrind --leak-check=full --track-origins=yes --suppressions=mlx42.supp ./$(NAME) $(ARG)

bonus: all

$(NAME): $(BIN_DIR) $(OBJS)
	@$(CC) $(C_FLAGS) $(OBJS) $(LIBFT) $(MLX42) $(C_LINK) $(INC) -o $(NAME)
	@echo Build complete!

$(BIN_DIR)%.o: %.c
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC)

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

.PHONY: all clean fclean re
