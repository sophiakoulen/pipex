/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:40:13 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/03 17:58:02 by skoulen          ###   ########.fr       */
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
}	t_pipex;

/* initialization */
int		init(int argc, char **argv, t_pipex *p);
int		init_redir(t_pipex *p, char **argv);
int		init_pipes(t_pipex *p);
int		init_cmds_and_paths(t_pipex *p, char **argv);

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
int		px_find_command(char *filename, char **envp, char **res);
char	**px_getpath(char **envp);
int		px_ispath(const char *str);
char	*px_path_combine(const char *path_1, const char *path_2);
int		px_check_abspath(char *abspath, char **res);

/* command splitter */
char	**split_cmd(char *str);

#endif