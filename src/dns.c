/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:35:36 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/02 21:09:20 by frosa-ma         ###   ########.fr       */
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

static void	__host(int i, int *pfd, t_data *data)
{
	char	*addr;
	char	*server;

	sleep(data->dns_obj[i]->interval);
	addr = ft_strtrim(data->dns_obj[i]->mon->addr, " \n\t");
	server = ft_strtrim(data->dns_obj[i]->domain, " \n\t");
	close(pfd[0]);
	dup2(pfd[1], STDOUT_FILENO);
	fflush(stdout);
	execlp("host", "host", addr, server);
}

static void	exec_child(int i, int *pfd, t_data *data)
{
	int		pid;

	pid = fork();
	if (pid == 0)
		__host(i, pfd, data);
}

void	monitore_dns(t_data data, int i)
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
					clean_dns_log(i, &data, 1);
				else
					dns_log(i, &data, 1);
				echo_dns(i, &data, 1);
				close(pfd[0]);
			}
			else
			{
				line = ft_gnl(pfd[0]);
				while (line)
				{
					if (is_present(line, "has address"))
					{
						data.dns_resp.ipv4 = get_ip(line);
						data.dns_resp.resolved = 1;
					}
					if (is_present(line, "has IPv6"))
						data.dns_resp.ipv6 = get_ip(line);
					free(line);
					line = ft_gnl(pfd[0]);
				}
				set_datetime(&data);
				if (data.f_simplify)
					clean_dns_log(i, &data, 0);
				else
					dns_log(i, &data, 0);
				echo_dns(i, &data, 0);
				close(pfd[0]);
			}
		}
	}
}
