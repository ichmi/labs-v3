/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:40:44 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/02 21:20:39 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webmon.h"

void	init_lists(t_data *data)
{
	char	**tab;
	char	*line;
	int		fd;
	
	fd = open("monitoring.db", O_RDONLY);
	line = ft_gnl(fd);
	while (line)
	{
		tab = ft_split(line, '\t');
		if (!ft_strncmp(tab[1], "HTTP", 5))
			data->http_list[data->http_i++] = ft_strtrim(line, "\n");
		else if (!ft_strncmp(tab[1], "PING", 5))
			data->ping_list[data->ping_i++] = ft_strtrim(line, "\n");
		else if (!ft_strncmp(tab[1], "DNS", 4))
			data->dns_list[data->dns_i++] = ft_strtrim(line, "\n");
		free(line);
		ft_free_matrix(tab);
		line = ft_gnl(fd);
	}
	close(fd);
}

static void	__init_data(t_data *data)
{
	data->n_http = 0;
	data->http_i = 0;
	data->http_resp.content_type = NULL;
	data->http_resp.server = NULL;
	data->n_ping = 0;
	data->ping_i = 0;
	data->n_dns = 0;
	data->dns_i = 0;
	data->dns_resp.resolved = 0;
	data->dns_resp.ipv4 = NULL;
	data->dns_resp.ipv6 = NULL;
	data->f_simplify = 0;
	data->n_mon = 0;
}

void	init_data(t_data *data)
{
	char	**tab;
	char	*line;
	int		fd;
	int		i;

	__init_data(data);
	fd = open("monitoring.db", O_RDONLY);
	line = ft_gnl(fd);
	while (line && *line != '\n')
	{
		tab = ft_split(line, '\t');
		i = 0;
		while (tab[i])
		{
			if (ft_strncmp(tab[i], "HTTP", 5) == 0)
				data->n_http++;
			if (ft_strncmp(tab[i], "PING", 5) == 0)
				data->n_ping++;
			if (ft_strncmp(tab[i], "DNS", 4) == 0)
				data->n_dns++;
			i++;
		}
		data->n_mon++;
		line = ft_gnl(fd);
	}
	close(fd);
	data->http_list = (char **)ft_calloc(data->n_http + 1, sizeof(char *));
	data->ping_list = (char **)ft_calloc(data->n_ping + 1, sizeof(char *));
	data->dns_list = (char **)ft_calloc(data->n_dns + 1, sizeof(char *));
	data->http_obj = (t_http **)malloc(data->n_http * sizeof(*data->http_obj));
	data->ping_obj = (t_ping **)malloc(data->n_ping * sizeof(*data->ping_obj));
	data->dns_obj = (t_dns **)malloc(data->n_dns * sizeof(*data->dns_obj));
}
