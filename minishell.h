/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:06:23 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:32:13 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

# define SUCCESS 0
# define FAILURE 1

# define HEREDOC_ABORT -2

# include "libft/libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define SIGF_INT
# define SIGF_QUIT
# define SIGF_REFRESH
# define SIGF_HDOC_INT

# ifndef HOST_NAME_MAX
#  define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
# endif

extern volatile sig_atomic_t	g_sigflags;

typedef enum e_err
{
	ERR_NONE = 0,
	ERR_NOT_FOUND,
	ERR_IS_DIR,
	ERR_NO_PERM,
	ERR_EXECFMT,
	ERR_ENOENT_PATH
}								t_err;

typedef struct s_env_list
{
	char						*key;
	char						*value;
	struct s_env_list			*next;
}								t_env_list;

typedef struct s_pipex
{
	int							pipe_fd[2];
	int							infile_fd;
	int							outfile_fd;
	char						**path_array;
	int							no_path_flag;
	int							last_status;
	int							status;
	int							heredoc_flag;
	t_env_list					*env_list;
	t_err						err;
	pid_t						pid1;
	pid_t						pid2;
	pid_t						pid;
}								t_pipex;
typedef enum s_token_type
{
	TOK_UNDEF = 0,
	WORD,
	PIPE,
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	AND
}								t_token_type;
typedef struct s_token_list
{
	t_token_type				type;
	char						*content;
	int							quoted;

	struct s_token_list			*next;
}								t_token_list;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	INIT
}								t_node_type;

typedef struct s_expand_ctx
{
	const char					*arg;
	t_pipex						*ps;
	char						*out;
	size_t						i;
	int							in_squote;
	int							in_dquote;
	int							should_break;
}								t_expand_ctx;

typedef struct s_arg
{
	char						*value;
	struct s_arg				*next;
}								t_arg;

typedef struct s_redir
{
	t_token_type				kind;
	char						*filename;
	char						*delim;
	int							quoted;
	struct s_redir				*next;
}								t_redir;

typedef struct s_ast
{
	t_node_type					type;
	char						**argv;
	t_arg						*arg_list;
	t_redir						*redirs;
	struct s_ast				*left;
	struct s_ast				*right;
}								t_ast;

typedef struct s_pipe_ctx
{
	int							fds[2];
	pid_t						left;
	pid_t						right;
}								t_pipe_ctx;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

int								apply_redirs(t_pipex *ps, t_redir *list);
void							ast_free_a(t_ast *n);
int								get_exit_status(int stR);
int								exec_cmd_node(t_ast *node_list, t_pipex *ps);
int								exec_cmd_pipe_node(t_ast *node_list,
									t_pipex *ps, char *line);
int								execute_ast(t_ast *node_list, t_pipex *ps,
									char *line);
void							init_history(void);
void							print_erro(char *err, char *msg);
char							**get_path_array(char **envp, t_pipex *px);
char							*find_command_path(char **path_array,
									char *cmd);
int								is_directory(const char *path);
int								execute_command(char *cmd_path, char **cmd_args,
									char **envp, t_pipex *ps);
char							**get_path_array(char **envp, t_pipex *px);
char							*resolve_command_path(char *cmd, t_pipex *ps);
int								execute(int argc, char **argv, t_pipex *ps);
char							*ft_search_access(char **envp);
int								init_pipe(char *line, char **envp);
void							free_pipex(t_pipex *ps);
void							setup_signal(void);
void							set_echoctl(int enable);
void							on_signal_child(int s_num);
void							on_signal_parents(int s_num);
void							setup_signal_child(void);
void							setup_signal_parents(void);
t_token_list					*create_token_list(char *line);
t_token_list					*create_new_token(char *line, size_t *index);
t_ast							*parse_pipeline(t_token_list **cur);
void							free_splits(char **cmd_args);
int								apply_redirs(t_pipex *ps, t_redir *list);
int								apply_one_redir(t_pipex *ps, t_redir *node);
t_ast							*parse_command(t_token_list **cur);
char							*ft_strjoin_g(char const *s1, char const *s2);
int								newline_or_not(char const *str);
char							*extract_line(char *stash);
char							*ft_strdup_g(const char *s1);
char							*update_stash(char *stash);
char							*get_next_line(int fd);
char							*process_backup(char **backup);
char							*read_to_backup(int fd, char *backup);
int								expand_command_line(t_ast *node, t_pipex *ps);
int								expand_command(t_ast *node, t_pipex *ps);
char							*expand_one(const char *arg, t_pipex *ps);
char							**list_to_argv(t_arg *list);
int								exec_builtin_exit(t_pipex *ps,
									t_ast *node_list);
int								exec_builtin_env(t_pipex *ps);
int								exec_builtin_unset(t_pipex *shell,
									t_ast *node_list);
int								exec_builtin_export(t_pipex *ps,
									t_ast *node_list);
int								exec_builtin_pwd(t_pipex *shell);
int								exec_builtin_cd(t_pipex *ps, t_ast *node_list);
int								exec_builtin_echo(t_ast *node_list);
int								update_env_value(t_pipex *ps, const char *key,
									const char *value);
int								is_builtin(char *argv);
int								exec_builtin(t_pipex *ps, t_ast *node_list);
int								is_parent_builtin(t_ast *node_list);
int								streq(const char *s, const char *lit);
void							ft_dprintf(int fd, char *filename,
									char *err_msg);
char							*ms_getenv_dup(t_pipex *ps, const char *key);
int								handle_exec_error(char **cmd_args, t_pipex *ps);
int								check_exec_permission(char *cmd_path,
									char **cmd_args, t_pipex *ps);
int								handle_directory_result(int directory,
									char **cmd_args, t_pipex *ps);
int								handle_no_cmd_path(t_pipex *ps,
									char **cmd_args);
int								validate_cmd_args(char **cmd_args, t_pipex *ps);
int								rn_wait_pid(pid_t pid);
pid_t							rn_fork_and_exec(t_pipex *ps, t_ast *node);
int								rn_status_from_wait(int st);
int								build_heredoc_fd(t_pipex *ps, const char *delim,
									int quoted);
char							*get_env_value(t_pipex *ps, const char *key);
int								is_valid_identifier(const char *s);
int								process_parsed_line(char *line, t_pipex *ps);
int								handle_signals_and_empty(char *line,
									t_pipex *ps);
int								process_line(char *line, t_pipex *ps);
int								run_line(t_pipex *ps);
int								try_parent_builtin_then_cleanup(
									t_ast *node_list,
									t_pipex *ps, char *line, int *handled);
int								build_nodes_from_line(const char *line,
									t_ast **out, t_pipex *ps);
int								validate_cmd_args_like_original(
									const char *line);
int								run_pipeline_and_cleanup(t_ast *node_list,
									t_pipex *ps);
int								post_read_signal_handle(char *line, t_pipex *ps,
									int *should_continue);
char							*ms_readline(void);
int								line_is_empty(const char *line);
int								run_node(t_pipex *ps, t_ast *node);
int								run_pipe_node(t_pipex *ps, t_ast *left,
									t_ast *right);
void							set_signals_parent_wait(void);
void							set_signals_prompt(void);
void							set_signals_child(void);
void							ms_run_left_child(t_pipex *ps, t_ast *left,
									int fds[2]);
void							ms_run_right_child(t_pipex *ps, t_ast *right,
									int fds[2]);
int								ms_wait_pipe_children(pid_t lp, pid_t rp);
void							exec_left_child(t_ast *node_list, t_pipex *ps,
									char *line, int fds[2]);
void							exec_right_child(t_ast *node_list, t_pipex *ps,
									char *line, int fds[2]);
int								wait_pipe_children(pid_t left, pid_t right);
int								is_env_start(int c);
int								is_var_char(int c);
char							*itoa_status(int st);
int								sb_append(char **dst, const char *piece,
									size_t len);
int								handle_quote_state(char c, int *in_squote,
									int *in_dquote, size_t *i);
int								split_key_value(const char *arg, char **out_key,
									char **out_val);
void							print_env_list_simple(t_env_list *head);
char							*find_in_path_string(const char *pathstr,
									const char *cmd);
int								cmd_push_arg_node(t_ast *cmd, const char *str);
void							append_redir(t_ast *cmd, t_redir *node);
t_redir							*make_heredoc_node(const char *delim,
									int quoted);
int								parse_redirection(t_ast *cmd, t_token_type kind,
									t_token_list *content);
void							parse_command_syntax_err(t_token_type type);
int								is_redir(t_token_type type);
int								parse_command_loop(t_token_list **cur,
									t_ast *cmd, int *seen);
void							consume(t_token_list **cur,
									t_token_type expected);
t_ast							*ast_new_pipe(t_ast *left, t_ast *right);
void							free_arg_list_a(t_arg *a);
void							free_redirs_a(t_redir *r);
t_ast							*ast_new_cmd(void);
int								run_simple_in_this_process(t_pipex *ps,
									t_ast *cmd);
int								is_space(char c);
int								is_metachar(char c);
t_token_type					classify_meta_tokens(char *line, size_t *index);
void							token_end(char *line, size_t *index,
									int *quoted);
int								is_all_space(const char *s);
void							read_one_heredoc_line(int wfd,
									const char *delim, int quoted, t_pipex *ps);
void							write_and_cleanup(int wfd, char *line,
									char *to_write, int quoted);
char							*expand_heredoc_line(char *line, int quoted,
									t_pipex *ps);
int								is_delim_line(char *line, const char *delim);
t_env_list						*env_push_front(t_env_list *head,
									const char *kv);
int								handle_other_token(t_ast *cmd,
									t_token_list **cur);
int								handle_redir_token(t_ast *cmd,
									t_token_list **cur, int *seen);
int								handle_heredoc_token(t_ast *cmd,
									t_token_list **cur, int *seen);
char							*strip_all_quotes(const char *s);
int								handle_word_token(t_ast *cmd,
									t_token_list **cur, int *seen);
void							free_token_list(t_token_list **list);
int								expand_one_loop(t_expand_ctx *ctx);
char							**ms_update_path_array_from_envlist(
									t_pipex *ps);
char							**get_envp_as_string(t_pipex *ps);
void							free_envp(char **envp);
int								process_one_arg(t_arg **pp, t_arg **cur,
									t_pipex *ps, int *removed);
void							replace_arg_value(t_arg *cur, char *orig,
									char *new_val);
void							remove_current_arg(t_arg **pp, t_arg **cur,
									char *orig, char *new_val);
int								should_remove_arg(const char *orig,
									const char *new_val);
void							init_flags(const char *orig, int *has_quote,
									int *has_dollar);
int								has_unclosed_quote(const char *line);
int								try_backslash_in_dquote(t_expand_ctx *ctx);
int								try_quote_or_dollar(t_expand_ctx *ctx);
int								handle_dollar(t_expand_ctx *ctx);
int								expand_one_step(t_expand_ctx *ctx);

#endif