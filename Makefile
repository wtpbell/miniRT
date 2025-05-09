vpath %.c src:src/container:src/math:src/math/vector

# === VARIABLES ===

NAME		:= miniRT
CC			:= cc
C_FLAGS		:= -Werror -Wall -Wextra
C_LINK		:= -ldl -lglfw -pthread -lm -Ofast

BIN_DIR		:= bin/
LIB_DIR		:= lib/
MLX42_DIR	:= $(LIB_DIR)mlx42
LIBFT_DIR	:= $(LIB_DIR)libft

MLX42		:= $(addprefix $(MLX42_DIR)/build/, libmlx42.a)
LIBFT		:= $(addprefix $(LIBFT_DIR)/, libft.a)

INC			:= -I ./include -I $(MLX42_DIR)/include -I $(LIBFT_DIR)/include
SRCS		:= main.c vector_init.c vector_helper.c vector_operation.c\
				vec_container.c vec_container_utils.c
OBJS		:= $(SRCS:%.c=$(BIN_DIR)%.o)

# === COMPILE RULES ===

all: $(LIBFT) $(MLX42) $(NAME)

val: C_FLAGS += -g3
val: clean all
	@valgrind --leak-check=full --track-origins=yes --suppressions=mlx42.supp ./$(NAME) $(ARG)

bonus: all

$(NAME): $(BIN_DIR) $(OBJS)
	@$(CC) $(OBJS) $(LIBFT) $(MLX42) $(C_LINK) $(INC) -o $(NAME)
	@echo Build complete!

$(BIN_DIR)%.o: %.c
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC)

$(LIBFT): $(LIBFT_DIR)
	@make -C $(LIBFT_DIR) bonus

$(MLX42): $(MLX42_DIR)
	@cmake -S $(MLX42_DIR) -B $(MLX42_DIR)/build && make -C $(MLX42_DIR)/build -j4

$(LIBFT_DIR): $(LIB_DIR)
	@git clone git@github.com:JoseJBoon/libft.git $(LIBFT_DIR)

$(MLX42_DIR): $(LIB_DIR)
	@git clone --branch v2.4.1 https://github.com/codam-coding-college/MLX42.git $(MLX42_DIR)

# === DIRECTORIES/GIT SUBMODULE ===

$(BIN_DIR):
	@mkdir $(BIN_DIR)

$(DB_DIR):
	@mkdir $(DB_DIR)

$(LIB_DIR):
	@mkdir $(LIB_DIR)

# === DIRECTORIES/GIT SUBMODULE ===

clean:
	@make -C $(LIBFT_DIR) clean
	@$(RM) $(OBJS)
	@$(RM) $(NAME)
	@echo Clean complete!

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	@$(RM) -r $(MLX42_DIR)/build
	@$(RM) -r $(BIN_DIR)
	@echo Fclean complete!

re: fclean all

.PHONY: all clean fclean re
