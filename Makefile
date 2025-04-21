CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
NAME = minishell

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
		error.c \
		validate_syntax.c \
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
OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

LIBFT_DIR = ./libft/libft
LIBFT = $(LIBFT_DIR)/libft.a

LIBS = -L$(LIBFT_DIR) -lft

OBJ_SUBDIRS = $(sort $(dir $(OBJS)))

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) bonus
	@echo "Libft compiled successfully."

$(OBJ_SUBDIRS):
	@mkdir -p $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_SUBDIRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS) $(LIBS)
	@echo "Minishell compiled successfully."

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)
	@echo "Object files removed."

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@echo "Executable and libraries removed."

re: fclean all
	@echo "Project recompiled."

.PHONY: all clean fclean re
