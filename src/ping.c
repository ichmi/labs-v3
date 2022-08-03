/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 07:21:25 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/03 15:52:08 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webmon.h"

static char	*get_ip(char *line)
{
	char	*buff;
	char	*ip;

	buff = ft_strnstr(line, "address", ft_strlen(line));
	ip = ft_strtrim(buff + 8, " \n\t");
	return (ip);
}

static void	send_icmp(int i, int *pfd, t_data *data)
{
	char	*addr;

	sleep(data->ping_obj[i]->interval);
	addr = ft_strtrim(data->ping_obj[i]->mon->addr, " \n\t");
	close(pfd[0]);
	dup2(pfd[1], STDOUT_FILENO);
	fflush(stdout);
	execlp("ping", "ping", "-c", "1", addr, NULL);
}

static void	exec_child(int i, int *pfd, t_data *data)
{
	int		pid;

	pid = fork();
	if (pid == 0)
		send_icmp(i, pfd, data);
}

void	monitore_ping(t_data data, int i)
{
	int	pfd[2];
	int	pid;

	while (1)
	{
		char	*line;
		int		wstatus;

		pipe(pfd);
		exec_child(i, pfd, &data);
		wait(&wstatus);
		close(pfd[1]);
		if (WIFEXITED(wstatus))
		{
			if (WEXITSTATUS(wstatus) != 0)
			{
				set_datetime(&data);
				if (data.f_simplify)
					clean_ping_log(i, &data, 1);
				else
					ping_log(i, &data, 1);
				echo_ping(i, &data, 1);
				close(pfd[0]);
			}
			else
			{
				line = ft_gnl(pfd[0]);
				while (line)
				{
					if (!ft_strncmp(line, "PING", 4))
					{
						char *temp = ft_strchr(line, '(');
						int size = 0;
						int	i = -1;
						while (temp[++i])
						{
							if (temp[i] == ')')
								break ;
							size++;
						}
						data.ping_resp.ipv4 = ft_substr(temp, 1, size - 1);
					}
					if (!ft_strncmp(line, "64 bytes from", ft_strlen("64 bytes from")))
					{
						char *second = ft_strnstr(line, "time=", ft_strlen(line));
						data.ping_resp.rtt = ft_atoi(second + 5);
					}
					free(line);
					line = ft_gnl(pfd[0]);
				}
				set_datetime(&data);
				if (data.f_simplify)
					clean_ping_log(i, &data, 0);
				else
					ping_log(i, &data, 0);
				echo_ping(i, &data, 0);
				close(pfd[0]);
			}
		}
	}
}
