/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:33:17 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/02 23:07:26 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webmon.h"

void	set_datetime(t_data *data)
{
	char 		**time_token;
	char 		*str;
	struct tm	*tm;
	time_t		t;

	str = (char *)ft_calloc(24 + 1, sizeof(char));
	t = time(NULL);
	tm = localtime(&t);
	strftime(str, 25, "%c", tm);
	time_token = ft_split(str, ' ');
	data->datetime.day_week = ft_strdup(time_token[0]);
	data->datetime.month = ft_strdup(time_token[1]);
	data->datetime.day = ft_strdup(time_token[2]);
	data->datetime.hour = ft_strdup(time_token[3]);
	data->datetime.fhour = ft_substr(time_token[3], 0, ft_strlen(time_token[3]) - 3);
	data->datetime.year = ft_strdup(time_token[4]);
	ft_free_matrix(time_token);
	free(str);
}

void	dns_disc_message(int i, t_data *data)
{
	int		pid;
	int		pfd[2];
	char	*args[12];
	char	*content;

	content = ft_strdup("{\"content\": \"ERROR (**");
	content = ft_strjoins(content, data->dns_obj[i]->mon->name);
	content = ft_strjoins(content, "**)\"}");
	args[0] = "curl";
	args[1] = "-s";
	args[2] = "-X";
	args[3] = "POST";
	args[4] = "-H";
	args[5] = "Content-Type: application/json";
	args[6] = "-H";
	args[7] = "authorization: MTAwNDE0MjA0NDk0OTc5NDk3Nw.GrGrqK.2kAr9sM0esgCbKhwMOTdyuKIniMcQKGRokJV08",
	args[8] = "-d";
	args[9] = content;
	args[10] = "https://discord.com/api/v9/channels/1004143274874900561/messages";
	args[11] = NULL;
	pid = fork();
	if (!pid)
	{
		pipe(pfd);
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		dup2(pfd[1], STDERR_FILENO);
		execvp("curl", args);
	}
	wait(NULL);
}

void	http_disc_message(int i, t_data *data)
{
	int		pid;
	int		pfd[2];
	char	*args[12];
	char	*content;

	content = ft_strdup("{\"content\": \"ERROR (**");
	content = ft_strjoins(content, data->http_obj[i]->mon->name);
	content = ft_strjoins(content, "**)\"}");
	args[0] = "curl";
	args[1] = "-s";
	args[2] = "-X";
	args[3] = "POST";
	args[4] = "-H";
	args[5] = "Content-Type: application/json";
	args[6] = "-H";
	args[7] = "authorization: MTAwNDE0MjA0NDk0OTc5NDk3Nw.GrGrqK.2kAr9sM0esgCbKhwMOTdyuKIniMcQKGRokJV08",
	args[8] = "-d";
	args[9] = content;
	args[10] = "https://discord.com/api/v9/channels/1004143274874900561/messages";
	args[11] = NULL;
	pid = fork();
	if (!pid)
	{
		pipe(pfd);
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		dup2(pfd[1], STDERR_FILENO);
		execvp("curl", args);
	}
	wait(NULL);
}

int	is_present(char *line, const char *param)
{
	// HTTP
	if (!ft_strncmp(param, "Content-Type", ft_strlen(param)))
	{
		if (!ft_strncmp(line, "Content-Type", ft_strlen(param))
			|| !ft_strncmp(line, "Content-type", ft_strlen(param))
			|| !ft_strncmp(line, "content-type", ft_strlen(param)))
			return (1);
	}
	if (!ft_strncmp(param, "Content-Length", ft_strlen(param)))
	{
		if (!ft_strncmp(line, "Content-Length", ft_strlen(param))
			|| !ft_strncmp(line, "Content-length", ft_strlen(param))
			|| !ft_strncmp(line, "content-length", ft_strlen(param)))
			return (1);
	}
	if (!ft_strncmp(param, "Server", ft_strlen(param)))
	{
		if (!ft_strncmp(line, "Server:", ft_strlen(param) + 1)
			|| !ft_strncmp(line, "server:", ft_strlen(param) + 1))
			return (1);
	}

	// DNS
	if (!ft_strncmp(param, "has address", ft_strlen(param)))
		if (ft_strnstr(line, "has address", ft_strlen(line)))
			return (1);
	if (!ft_strncmp(param, "has IPv6", ft_strlen(param)))
		if (ft_strnstr(line, "has IPv6", ft_strlen(line)))
			return (1);
	return (0);
}
