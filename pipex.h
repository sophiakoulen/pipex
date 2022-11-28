/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:40:13 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/28 17:00:10 by skoulen          ###   ########.fr       */
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

typedef struct s_px_error
{
	int	status;
	int	errno_value;
}	t_px_error;

typedef struct s_command
{
	char	*raw_string;
	char	*infile;
	char	*outfile;
	char	**args;
	char	*exec_path;
	int		fdin;
	int		fdout;
	int		pid;
}	t_command;

typedef struct s_command_list
{
	int			size;
	t_command	*arr;
}	t_command_list;

# define PX_SUCCESS	0
# define PX_CMD_NOT_FOUND 1
# define PX_SEE_ERRNO 2
# define PX_WRONG_USAGE 3

# define READ 0
# define WRITE 1

/* utils 1*/
int			px_ispath(const char *str);
t_px_error	px_find_command(char *filename, char **envp, char **res);

/* utils 2*/
void		launch_child(t_command_list	*cl, int **pipes, int index);

/* utils 3*/
void		launch_all_children(t_command_list *cl, int **pipes);
int			wait_for_all_children(void);
t_px_error	px_parse_args(int argc, char **argv, t_command_list *cl);
int			px_exit_status(int status);

/* utils 4 */
t_px_error	px_split_command(char *str, char ***res);

/* utils 5 */
t_px_error	prepare_program(t_command_list *cl, int **pipes, int index, int n);

/* utils 6 */
t_px_error	open_pipes(int **pipes, int n);
void		close_unused_pipe_ends(int **pipes, int index, int n);
void		close_used_pipe_ends(int **pipes, int index, int n);
void		close_pipes(int **pipes, int n);
t_px_error	allocate_pipes(int ***pipes, int n);

/* utils 7 */
void		cleanup_command(t_command *program);
void		cleanup_command_list(t_command_list *cl);
void		cleanup_pipes_halfway(int **pipes, int n);
void		cleanup_pipes(int **pipes, int n);
void		cleanup_args(char **tab);

/* utils 8 */
void		px_print_error(const char *str, t_px_error err);
t_px_error	px_set_error(int status);

#endif