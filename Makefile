CC = cc
CFLAGS = -g -Wall -Wextra -Werror
LDFLAGS = -lreadline
NAME = minishell

SRCS = minishell.c \
		command_handler.c \
		command_handler_utils.c \
		shell_setup.c \
		shell_setup_utils.c \
		expand_variables.c \
		expand_variables_utils.c \
		input.c \
		input_with_echo.c \
		input_with_echo_utils.c \
		input_utils.c \
		input_parsing.c \
		path_handler.c \
		cleanup.c \
		expansions.c \
		pipeline.c \
		execute_child_process.c \
		execute_parent_process.c \
		pipe_utils.c \
		pipe_split.c \
		pipe_processing.c \
		pipe_processing_utils.c \
		pipe_redirection.c \
		pipe_redirection_utils.c \
		quotes.c \
		quotes_utils.c \
		redirections.c \
		redirections_utils.c \
		redirections_processing.c \
		tokenize.c \
		tokenize_utils.c \
		error.c \
		validate_syntax.c \
		validate_syntax_utils.c \
		./builtins/builtin_utils.c \
		./builtins/cd/builtin_cd.c \
		./builtins/cd/builtin_cd_utils.c \
		./builtins/cd/builtin_oldpwd.c \
		./builtins/echo/builtin_echo.c \
		./builtins/echo/builtin_echo_utils.c \
		./builtins/env/builtin_env.c \
		./builtins/export/builtin_export.c \
		./builtins/export/builtin_export_utils.c \
		./builtins/export/builtin_export_utils2.c \
		./builtins/exit/builtin_exit.c \
		./builtins/exit/builtin_exit_utils.c \
		./builtins/pwd/builtin_pwd.c \
		./builtins/unset/builtin_unset.c \
		utils_dump.c \
		process_execution.c \
		shell_loop.c \
		signals.c

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
