# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/16 20:39:38 by kotadashiru       #+#    #+#              #
#    Updated: 2025/12/05 13:41:01 by rufurush         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC =	apply_one_redir_utils.c \
		execute_utils.c	\
		executor.c	\
		export_utils.c	\
		meta_token_end_utils.c	\
		redirect.c	\
		unset_buildin.c	\
		cd_buildin.c	\
		execute_utils2.c	\
		exit_buildin.c	\
		find_in_path_strings.c	\
		parse_command_utils.c	\
		resolve_command_path.c	\
		signal_utils.c			\
		redirect_utils.c	\
		utils.c	\
		echo_buildin.c	\
		execute_utils3.c	\
		expand.c	\
		get_next_line.c	\
		parse_pipeline.c	\
		run_node_utils.c	\
		env_buildin.c	\
		execute_utils4.c	\
		expand_command.c	\
		expand_command_utils.c \
		get_next_line_utils.c	\
		parse_utils.c	\
		signal.c	\
		env_utils.c	\
		execute_utils5.c	\
		expand_one.c	\
		init_buildin.c	\
		parse_utils1.c	\
		token_end_utils.c	\
		execute.c	\
		execute_utils6.c	\
		expand_utils.c	\
		list_to_argv.c	\
		parse_utils2.c	\
		token_utils.c	\
		execute_command_utils.c	\
		execute_utils7.c	\
		export_buildin.c	\
		main.c	\
		pwd_builtin.c	\
		tokens.c	\
		parse_envp.c	\
		libft/ft_calloc.c	\
		libft/ft_isascii.c	\
		libft/ft_itoa.c	\
		libft/ft_memcpy.c	\
		libft/ft_putchar_fd.c	\
		libft/ft_putstr_fd.c	\
		libft/ft_strdup.c	\
		libft/ft_strlcpy.c	\
		libft/ft_strncpy.c	\
		libft/ft_strtrim.c	\
		libft/ft_toupper.c	\
		libft/ft_atoi.c	\
		libft/ft_isalnum.c	\
		libft/ft_isdigit.c	\
		libft/ft_memchr.c	\
		libft/ft_memmove.c	\
		libft/ft_putendl_fd.c	\
		libft/ft_split.c	\
		libft/ft_strjoin.c	\
		libft/ft_strlen.c	\
		libft/ft_strnstr.c	\
		libft/ft_substr.c	\
		libft/ft_bzero.c	\
		libft/ft_isalpha.c	\
		libft/ft_isprint.c	\
		libft/ft_memcmp.c	\
		libft/ft_memset.c	\
		libft/ft_putnbr_fd.c	\
		libft/ft_strchr.c	\
		libft/ft_strlcat.c	\
		libft/ft_strncmp.c	\
		libft/ft_strrchr.c	\
		libft/ft_tolower.c
OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
