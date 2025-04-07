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

LIBFT_DIR = ./libft/libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "Libft compiled."

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BUILTINS_DIRS): | $(OBJ_DIR)
	@mkdir -p $(BUILTINS_DIRS)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(BUILTINS_DIRS)
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
