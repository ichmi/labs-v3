/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 14:15:46 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/01 16:44:35 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>

#include <ctype.h>
#include <time.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../libft/libft.h"

typedef struct s_mon
{
	char	*name;
	char	*prot;
	char	*addr;
}		t_mon;

typedef struct s_http
{
	t_mon	*mon;
	char	*method;
	int		expected_status_code;
	int		interval;
}		t_http;

typedef struct s_ping
{
	t_mon	*mon;
	int		interval;
}		t_ping;

typedef struct s_dns
{
	t_mon	*mon;
	int		interval;
	char	*domain;
}		t_dns;

typedef struct s_resp
{
	char	*status_code_response;
	char	**token;
	char	*prot;
	int		code;
	char	*content_type;
	char	*content_length;
	char	*server;
}		t_http_resp;

typedef struct s_dt
{
		char	*day_week;
		char	*day;
		char	*month;
		char	*year;
		char	*hour;
		char	*fhour;
}		t_dt;

typedef struct s_data
{
	char	**http_objs;
	char	**ping_objs;
	char	**dns_objs;
	int		http_i;
	int		ping_i;
	int		dns_i;

	t_http	**http_obj;
	t_ping	**ping_obj;
	t_dns	**dns_obj;
	t_http_resp	http_resp;
	t_dt	datetime;

	double	req_time;
	int		n_mon;
	int		n_http;
	int		n_ping;
	int		n_dns;
	int		f_simplify;
}		t_data ;

static void	__init_data(t_data *data)
{
	data->f_simplify = 0;
	data->n_mon = 0;
	data->n_http = 0;
	data->http_i = 0;
	data->n_ping = 0;
	data->ping_i = 0;
	data->n_dns = 0;
	data->dns_i = 0;
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
	data->http_objs = (char **)ft_calloc(data->n_http + 1, sizeof(char *));
	data->ping_objs = (char **)ft_calloc(data->n_ping + 1, sizeof(char *));
	data->dns_objs = (char **)ft_calloc(data->n_dns + 1, sizeof(char *));

	data->http_obj = (t_http **)malloc(data->n_http * sizeof(*data->http_obj));
	data->ping_obj = (t_ping **)malloc(data->n_ping * sizeof(*data->ping_obj));
	data->dns_obj = (t_dns **)malloc(data->n_dns * sizeof(*data->dns_obj));
}
//////

// utils
void	display_tab(t_data data)
{
	printf("\x1b[38;5;9m=========== HTTP\x1b[0m\n");
	printf("char *https[%d] = {\n", data.n_http);
	for (int i = 0; data.http_objs[i]; ++i) {
		printf("\t\"%s\",\n", data.http_objs[i]);
	}
	printf("\tNULL\n};\n");

	printf("\n\x1b[38;5;9m=========== PING\x1b[0m\n");
	printf("char *pings[%d] = {\n", data.n_ping);
	for (int i = 0; data.ping_objs[i]; ++i) {
		printf("\t\"%s\",\n", data.ping_objs[i]);
	}
	printf("\tNULL\n};\n");

	printf("\n\x1b[38;5;9m=========== DNS\x1b[0m\n");
	printf("char *dns[%d] = {\n", data.n_dns);
	for (int i = 0; data.dns_objs[i]; ++i) {
		printf("\t\"%s\",\n", data.dns_objs[i]);
	}
	printf("\tNULL\n};\n");
}
void	display_objects(t_data data)
{
	printf("\x1b[38;5;9m=========== HTTP\x1b[0m\n");
	for (int i = 0; data.http_obj[i]; ++i) {
		printf("http(%s, %s, %s, %s, %d, %d)\n",
			data.http_obj[i]->mon->name,
			data.http_obj[i]->mon->prot,
			data.http_obj[i]->mon->addr,
			data.http_obj[i]->method,
			data.http_obj[i]->expected_status_code,
			data.http_obj[i]->interval);
	}
	printf("\n\x1b[38;5;9m=========== PING\x1b[0m\n");
	for (int i = 0; data.ping_obj[i]; ++i) {
		printf("ping(%s, %s, %s, %d)\n",
			data.ping_obj[i]->mon->name,
			data.ping_obj[i]->mon->prot,
			data.ping_obj[i]->mon->addr,
			data.ping_obj[i]->interval);
	}
	printf("\n\x1b[38;5;9m=========== DNS\x1b[0m\n");
	for (int i = 0; data.dns_obj[i]; ++i) {
		printf("dns(%s, %s, %s, %d, %s)\n",
			data.dns_obj[i]->mon->name,
			data.dns_obj[i]->mon->prot,
			data.dns_obj[i]->mon->addr,
			data.dns_obj[i]->interval,
			data.dns_obj[i]->domain);
	}
}

void	init_queries(t_data *data)
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
			data->http_objs[data->http_i++] = ft_strtrim(line, "\n");
		else if (!ft_strncmp(tab[1], "PING", 5))
			data->ping_objs[data->ping_i++] = ft_strtrim(line, "\n");
		else if (!ft_strncmp(tab[1], "DNS", 4))
			data->dns_objs[data->dns_i++] = ft_strtrim(line, "\n");
		free(line);
		ft_free_matrix(tab);
		line = ft_gnl(fd);
	}
	close(fd);
}

t_http	*get_http_obj(char **tab)
{
	t_mon	*obj;
	t_http	*http_obj;
	
	obj = (t_mon *)malloc(1 * sizeof(*obj));
	obj->name = ft_strdup(tab[0]);
	obj->prot = ft_strdup(tab[1]);
	obj->addr = ft_strdup(tab[2]);
	http_obj = (t_http *)malloc(1 * sizeof(*http_obj));
	http_obj->mon = obj;
	http_obj->method = ft_strdup(tab[3]);
	http_obj->expected_status_code = ft_atoi(tab[4]);
	http_obj->interval = ft_atoi(tab[5]);
	return (http_obj);
}
t_ping	*get_ping_obj(char **tab)
{
	t_mon	*obj;
	t_ping	*ping_obj;
	
	obj = (t_mon *)malloc(1 * sizeof(*obj));
	obj->name = ft_strdup(tab[0]);
	obj->prot = ft_strdup(tab[1]);
	obj->addr = ft_strdup(tab[2]);
	ping_obj = (t_ping *)malloc(1 * sizeof(*ping_obj));
	ping_obj->mon = obj;
	ping_obj->interval = ft_atoi(tab[3]);
	return (ping_obj);
}
t_dns	*get_dns_obj(char **tab)
{
	t_mon	*obj;
	t_dns	*dns_obj;
	
	obj = (t_mon *)malloc(1 * sizeof(*obj));
	obj->name = ft_strdup(tab[0]);
	obj->prot = ft_strdup(tab[1]);
	obj->addr = ft_strdup(tab[2]);
	dns_obj = (t_dns *)malloc(1 * sizeof(*dns_obj));
	dns_obj->mon = obj;
	dns_obj->interval = ft_atoi(tab[3]);
	dns_obj->domain = ft_strdup(tab[4]);
	return (dns_obj);
}
void	init_mon_objs(t_data *data)
{
	char	**tab;
	int		i;

	i = -1;
	while (data->http_objs[++i])
	{
		tab = ft_split(data->http_objs[i], '\t');
		data->http_obj[i] = get_http_obj(tab);
		ft_free_matrix(tab);
	}
	i = -1;
	while (data->ping_objs[++i])
	{
		tab = ft_split(data->ping_objs[i], '\t');
		data->ping_obj[i] = get_ping_obj(tab);
		ft_free_matrix(tab);
	}
	i = -1;
	while (data->dns_objs[++i])
	{
		tab = ft_split(data->dns_objs[i], '\t');
		data->dns_obj[i] = get_dns_obj(tab);
		ft_free_matrix(tab);
	}
}

void	exec_curl(int i, int *pfd, t_data *data)
{
	char	*addr;

	sleep(data->http_obj[i]->interval);
	addr = ft_strtrim(data->http_obj[i]->mon->addr, " \n\t");
	close(pfd[0]);
	dup2(pfd[1], STDOUT_FILENO);
	fflush(stdout);
	execlp("curl", "curl", "-Is", addr);
	perror("CANT FIND PATH");
	exit(1);
}

void	exec_child(int i, int *pfd, t_data *data)
{
	clock_t	start;
	clock_t	end;
	int		pid;

	pid = fork();
	start = clock();
	if (pid == 0)
		exec_curl(i, pfd, data);
	wait(NULL);
	end = clock();
	data->req_time = (double)(end - start) / CLOCKS_PER_SEC;
}

int	is_present(char *line, const char *param)
{
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
	return (0);
}

char	*get_content_type(char *line)
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
char	*get_server(char *line)
{
	char	**tokens;
	char	*buff;

	tokens = ft_split(line, ':');
	buff = ft_strtrim(tokens[1], " \n");
	ft_free_matrix(tokens);
	return (buff);
}

void	set_datetime(t_data *data)
{
	char 		**time_token;
	char 		*str; //// cuidar quando for entre os dias 1-9 != 10-31
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

void	simplified_log(int i, t_data *data)
{
	char	*hour;
	char	*name;
	char	*addr;
	int		code;
	int		fd;

	hour = data->datetime.hour;
	name = data->http_obj[i]->mon->name;
	addr = data->http_obj[i]->mon->addr;
	code = data->http_resp.code;
	fd = open("monitoring.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	ft_putstr_fd(hour, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(name, fd);
	ft_putstr_fd(" ", fd);
	ft_putnbr_fd(data->http_obj[i]->expected_status_code, fd);
	ft_putstr_fd(":", fd);
	ft_putnbr_fd(code, fd);
	ft_putstr_fd(" ", fd);
	ft_putendl_fd(addr, fd);
	close(fd);
}

void	log_all(int i, t_data *data)
{
	char	buff[9];
	int		fd;

	fd = open("monitoring.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	ft_putstr_fd(data->datetime.day_week, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(data->datetime.day, fd);
	ft_putstr_fd("-", fd);
	ft_putstr_fd(data->datetime.month, fd);
	ft_putstr_fd("-", fd);
	ft_putstr_fd(data->datetime.year + 2, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(data->datetime.hour, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(data->http_obj[i]->mon->name, fd);
	ft_putstr_fd(" ", fd);
	ft_putnbr_fd(data->http_obj[i]->expected_status_code, fd);
	ft_putstr_fd(":", fd);
	ft_putnbr_fd(data->http_resp.code, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(data->http_resp.prot, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(data->http_obj[i]->method, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(data->http_obj[i]->mon->addr, fd);
	ft_putstr_fd(" ", fd);
	snprintf(buff, 9, "%f", data->req_time);
	ft_putstr_fd(buff, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(data->http_resp.content_type, fd);
	ft_putstr_fd(" ", fd);
	if (data->http_resp.server)
		ft_putstr_fd(data->http_resp.server, fd);
	else
		ft_putendl_fd("- ", fd);
	close(fd);
}

void	display(int i, t_data *data)
{
	char	*hour;
	char	*name;
	char	*addr;
	int		code;
	int		expected_code;

	hour = data->datetime.fhour;
	name = data->http_obj[i]->mon->name;
	addr = data->http_obj[i]->mon->addr;
	code = data->http_resp.code;
	expected_code = data->http_obj[i]->expected_status_code;
	printf("%s ", hour);
	printf("\x1b[38;5;8m%s\x1b[0m ", name);
	if (code == expected_code)
		printf("\x1b[38;5;10m%d\x1b[0m ", code);
	else
		printf("\x1b[38;5;9m%d\x1b[0m ", code);
	printf("%s\n", addr);
}

void	exec(t_data data, int i)
{
	int	pfd[2];
	int	pid;

	while (1)
	{
		char	*line;

		pipe(pfd);
		exec_child(i, pfd, &data);
		close(pfd[1]);

		// Separacao do status code:   HTTP/1.1 200 OK  ->  http_resp("HTTP/1.1", 200, "OK")
		data.http_resp.status_code_response = ft_gnl(pfd[0]);
		data.http_resp.token = ft_split(data.http_resp.status_code_response, ' ');
		data.http_resp.prot = data.http_resp.token[0];
		data.http_resp.code = ft_atoi(data.http_resp.token[1]);

		data.http_resp.content_type = NULL;
		data.http_resp.server = NULL;
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
			simplified_log(i, &data);
		else
			log_all(i, &data);
		display(i, &data);
		close(pfd[0]);
	}
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	init_data(&data);
	if (av[1])
		if (!ft_strncmp(av[1], "--simplify", 11))
			data.f_simplify = 1;
	init_queries(&data);
	init_mon_objs(&data);


	int		pid[data.n_http];

	i = -1;
	while (++i < data.n_http)
	{
		pid[i] = fork();
		if (pid[i] == 0)
			exec(data, i);
	}
	wait(NULL);
	




	return (0);
}
