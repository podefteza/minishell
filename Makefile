CC = cc

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -lreadline

SRCS = minishell.c \
		command_handler.c \
		fork_processes.c \
		shell_setup.c \
		expand_variables.c \
		format_input.c \
		path_handler.c \
		cleanup.c \
		./builtins/builtin_setup.c \
		./builtins/builtin_cd.c \
		./builtins/builtin_echo.c \
		./builtins/builtin_export.c \
		./builtins/builtin_exit.c \
		./builtins/builtin_pwd.c \
		./builtins/builtin_unset.c

OBJ_DIR = obj
BUILTINS_DIR = $(OBJ_DIR)/builtins

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

NAME = minishell

LIBFT_DIR = ./libft/libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "Libft compiled."

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BUILTINS_DIR):
	@mkdir -p $(BUILTINS_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(BUILTINS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) -L$(LIBFT_DIR) -lft
	@echo "Compilation successful."

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)
	@echo "Object files removed."

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@echo "Executable removed."

re: fclean all
	@echo "Recompiled all files."

.PHONY: all clean fclean re
