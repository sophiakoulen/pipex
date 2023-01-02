/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:40:13 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/02 16:54:27 by skoulen          ###   ########.fr       */
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

# include <sys/types.h>
# include <signal.h>

# include "libft.h"
# include "ft_printf.h"

typedef	struct s_pipex
{
	int		input_redir;
	int		output_redir;
	int		*statuses;
	int		*pids;
	char	***cmds;
	char	**paths;
	int		**pipes;
}	t_pipex;

int 	init(int argc, char **argv, t_pipex *p);
void	cleanup_pipex(int argc, t_pipex *p);
void	cleanup_pipes(int n, int **pipes);
void	cleanup_strs(char **strs);
void	cleanup_cmds(int n, char ***cmds);
void	cleanup_paths(int n, char **paths);
void	cleanup_pids(int *pids);
void	cleanup_statuses(int *statuses);
int		exec_pipeline(int argc, t_pipex *p);
int		close_unused_fd(int i, int n, t_pipex *p);
int		close_used_fd(int i, int n, t_pipex *p);
int		redirect(int i, int n, t_pipex *p);
int		is_broken(int i, int n, t_pipex *p);
int		compute_return_value(int status);

int		px_find_command(char *filename, char **envp, char **res);
char	**px_getpath(char **envp);
int		px_ispath(const char *str);
char	*px_path_combine(const char *path_1, const char *path_2);
int		px_check_abspath(char *abspath, char **res);

#endif