/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:30:23 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/02 21:09:09 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webmon.h"

static char	*get_server(char *line)
{
	char	**tokens;
	char	*buff;

	tokens = ft_split(line, ':');
	buff = ft_strtrim(tokens[1], " \n");
	ft_free_matrix(tokens);
	return (buff);
}

static char	*get_content_type(char *line)
{
	char	**tokens;
	char	**stokens;
	char	*buff;

	tokens = ft_split(line, ':');
	if (ft_strchr(tokens[1], ';'))
	{
		stokens = ft_split(tokens[1], ';');
		buff = ft_strtrim(stokens[0], " \n");
		ft_free_matrix(tokens);
		ft_free_matrix(stokens);
	}
	else
	{
		buff = ft_strtrim(tokens[1], " \n");
		ft_free_matrix(tokens);
	}
	return (buff);
}

static void	send_request(int i, int *pfd, t_data *data)
{
	char	*addr;
	char	*method;

	sleep(data->http_obj[i]->interval);
	addr = ft_strtrim(data->http_obj[i]->mon->addr, " \n\t");
	close(pfd[0]);
	dup2(pfd[1], STDOUT_FILENO);
	fflush(stdout);
	method = data->http_obj[i]->method;
	execlp("curl", "curl", "-IsX", method, addr, NULL);
}

static void	exec_child(int i, int *pfd, t_data *data)
{
	int		pid;

	pid = fork();
	if (!pid)
		send_request(i, pfd, data);
}

void	monitore_http(t_data data, int i)
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
		// HTTP/1.1 200 OK  ->  http_resp("HTTP/1.1", 200, "OK")
		data.http_resp.status_code_response = ft_gnl(pfd[0]);
		data.http_resp.token = ft_split(data.http_resp.status_code_response, ' ');
		data.http_resp.prot = data.http_resp.token[0];
		data.http_resp.code = ft_atoi(data.http_resp.token[1]);

		line = ft_gnl(pfd[0]);
		while (line)
		{
			if (is_present(line, "Content-Type"))
				data.http_resp.content_type = get_content_type(line);
			if (is_present(line, "Server"))
				data.http_resp.server = get_server(line);
			free(line);
			line = ft_gnl(pfd[0]);
		}
		set_datetime(&data);
		if (data.f_simplify)
			clean_http_log(i, &data);
		else
			http_log(i, &data);
		echo_http(i, &data);
		close(pfd[0]);
	}
}