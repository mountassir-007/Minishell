/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahel-mou@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 16:28:13 by ahel-mou          #+#    #+#             */
/*   Updated: 2022/03/21 18:13:24 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	if (sig == SIGINT && cld_proc != 0)
	{
		kill(cld_proc, SIGCONT);
		write(1, "\n", 1);
	}
	else if (sig == SIGINT && cld_proc == 0)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_sigquit(int sig)
{
	if (sig == SIGQUIT && cld_proc != 0)
	{
		if (!kill(cld_proc, SIGCONT))
			ft_putstr_fd("Quit: 3\n", 1);
	}
	else if (sig == SIGQUIT && cld_proc == 0)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static int	empty_input(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->input_error)
		return (1);
	if (!shell->cmd[0])
		return (1);
	while (shell->cmd[0][i]
		&& (white_spaces(shell->cmd[0][i]) || shell->cmd[0][i] == '\n'))
	i++;
	if (shell->cmd[0][i] == '\0')
	 	return (1);
	return (0);
}

int	main(int c, char **v, char **env)
{
	t_shell	shell;
	char	*rl_return;
	v = NULL;
	
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	// signal(SIGQUIT, SIG_IGN); // ignores the SIGINT with SIG_IGN
	if (c > 1)
		return (printf("no need for arguments\n"));
	init_structure(&shell, env);
	shell_levels(&shell); // increment the SHLVL env-variable
	while (1)
	{
		rl_return = readline("Minishell_$:");
		if (!rl_return)
			exit(1);
		add_history(rl_return);
		reinit_structure(&shell); // we reinit the shell args after every command
		split_commands(&shell, rl_return); // save the splited input in **shell->cmd
		if (!empty_input(&shell))
			start_shell(&shell);
	}
	free_struct(&shell);
	return (0);
}
