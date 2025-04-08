CC = cc

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -lreadline

SRCS = minishell.c \
		command_handler.c \
		shell_setup.c \
		expand_variables.c \
		input.c \
		path_handler.c \
		cleanup.c \
		expansions.c \
		pipeline.c \
		pipeline_utils.c \
		quotes.c \
		redirections.c \
		tokenize.c \
		./builtins/builtin_setup.c \
		./builtins/cd/builtin_cd.c \
		./builtins/cd/builtin_cd_utils.c \
		./builtins/echo/builtin_echo.c \
		./builtins/echo/builtin_echo_utils.c \
		./builtins/env/builtin_env.c \
		./builtins/export/builtin_export.c \
		./builtins/exit/builtin_exit.c \
		./builtins/exit/builtin_exit_utils.c \
		./builtins/pwd/builtin_pwd.c \
		./builtins/unset/builtin_unset.c

OBJ_DIR = obj
BUILTINS_DIRS = $(OBJ_DIR)/builtins $(OBJ_DIR)/builtins/cd $(OBJ_DIR)/builtins/echo \
	$(OBJ_DIR)/builtins/env $(OBJ_DIR)/builtins/export $(OBJ_DIR)/builtins/exit \
	$(OBJ_DIR)/builtins/pwd $(OBJ_DIR)/builtins/unset

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
NAME = minishell

# Paths to libft and ft_printf
LIBFT_DIR = ./libft/libft
LIBFT = $(LIBFT_DIR)/libft.a

FT_PRINTF_DIR = ./libft/libft/ft_printf/ft_printf  # Adjust path if needed
FT_PRINTF = $(FT_PRINTF_DIR)/libftprintf.a

# Combined libraries (libft + ft_printf)
LIBS = -L$(LIBFT_DIR) -lft -L$(FT_PRINTF_DIR) -lftprintf

all: $(LIBFT) $(FT_PRINTF) $(NAME)

# Build libft (with bonus)
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) bonus
	@echo "Libft compiled successfully."

# Build ft_printf
$(FT_PRINTF):
	@$(MAKE) -C $(FT_PRINTF_DIR)
	@echo "ft_printf compiled successfully."

# Create object directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BUILTINS_DIRS): | $(OBJ_DIR)
	@mkdir -p $(BUILTINS_DIRS)

# Compile minishell objects
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(BUILTINS_DIRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Link minishell with libft + ft_printf
$(NAME): $(OBJS) $(LIBFT) $(FT_PRINTF)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) $(LIBS)
	@echo "Minishell compiled successfully."

# Clean everything
clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(MAKE) -C $(FT_PRINTF_DIR) clean
	@rm -rf $(OBJ_DIR)
	@echo "Object files removed."

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(MAKE) -C $(FT_PRINTF_DIR) fclean
	@rm -f $(NAME)
	@echo "Executable and libraries removed."

re: fclean all
	@echo "Project recompiled."

.PHONY: all clean fclean re
