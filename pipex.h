/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:40:13 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/10 14:07:17 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <sys/errno.h>
# include <unistd.h>
# include <sys/wait.h>

# include "libft.h"
# include "ft_printf.h"
# include "get_next_line.h"

//the default value to use when PATH is not set
# define DEFAULT_PATH "/usr/bin:bin"

typedef struct s_parse_state
{
	int	squote;
	int	dquote;
	int	escaped;
}	t_parse_state;

//Careful! having paths and path variables, and of same type!!
typedef struct s_pipex
{
	int		input_fd;
	int		output_fd;
	int		*statuses;
	int		*pids;
	char	***cmds;
	char	**paths;
	int		**pipes;
	int		n_cmds;
	int		n_pipes;
	char	**path;
	int		heredoc;
	char	**raw_list;
	const char	*delim;
	int		heredoc_pipe[2];
	int		stdout;
}	t_pipex;

/* initialization */
int		init(int argc, char **argv, t_pipex *p);
int		init_path(t_pipex *p);
int		init_redir(t_pipex *p);
int		init_pipes(t_pipex *p);
int		init_cmds_and_paths(t_pipex *p);

/* cleanup */
void	cleanup_pipex(t_pipex *p);
void	cleanup_pipes(int n, int **pipes);
void	cleanup_strs(char **strs);
void	cleanup_cmds(int n, char ***cmds);
void	cleanup_paths(int n, char **paths);

/* execution */
int		exec_pipeline(t_pipex *p);
int		close_unused_fd(int i, t_pipex *p);
int		close_all_fd(t_pipex *p);
int		redirect(int i, t_pipex *p);
int		compute_return_value(int status);

/* command finder */
int		find_cmd(char **path, char *filename, char **res);
char	**extract_path(char **envp);
int		has_slashes(const char *str);
char	*concat_slash(const char *str1, const char *str2);
int		file_ok(char *filename, char **res);

/* command splitter */
char	**split_cmd(char *str);
char	*iter(char **ptr);
char	*trim(char *str);
int		word_len(char *str);
int		next_word(char **str);

/* heredoc */
void	heredoc(t_pipex *p);
int	redirect_heredoc(t_pipex *p);

#endif