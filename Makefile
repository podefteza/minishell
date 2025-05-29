CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
NAME = minishell

SRCS = minishell.c \
		./builtins/builtin_utils.c \
		./builtins/cd/builtin_cd.c \
		./builtins/cd/builtin_cd_utils.c \
		./builtins/cd/builtin_oldpwd.c \
		./builtins/echo/builtin_echo.c \
		./builtins/env/builtin_env.c \
		./builtins/export/builtin_export.c \
		./builtins/export/builtin_export_utils.c \
		./builtins/export/builtin_export_utils2.c \
		./builtins/exit/builtin_exit.c \
		./builtins/exit/builtin_exit_utils.c \
		./builtins/pwd/builtin_pwd.c \
		./builtins/unset/builtin_unset.c \
		./builtins/unset/builtin_unset_utils.c \
		./execution/child_process.c \
		./execution/command_execution.c \
		./execution/command_handler_utils.c \
		./execution/command_handler.c \
		./execution/execute_final_command.c \
		./execution/finalize_execution.c \
		./execution/path_handler.c \
		./expansions/expand_variables_utils.c \
		./expansions/expand_variables.c \
		./expansions/expansions.c \
		./parser/input_utils.c \
		./parser/input.c \
		./parser/process_heredoc.c \
		./parser/quotes_utils.c \
		./parser/quotes.c \
		./parser/tokenize_utils.c \
		./parser/tokenize.c \
		./parser/validate_syntax_utils.c \
		./parser/validate_syntax.c \
		./pipes/pipe_split.c \
		./pipes/pipe_utils.c \
		./redirections/redirections_utils.c \
		./redirections/redirections.c \
		./shell/shell_loop.c \
		./shell/shell_setup_utils.c \
		./shell/shell_setup.c \
		./utils/cleanup.c \
		./utils/error.c \
		./utils/general_utils.c \
		./utils/signals.c

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
