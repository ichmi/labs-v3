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

typedef struct s_http_resp
{
	char	*status_code_response;
	char	**token;
	char	*prot;
	int		code;
	char	*content_type;
	char	*content_length;
	char	*server;
}		t_http_resp;

typedef struct s_dns_resp
{
	char	*ipv4;
	char	*ipv6;
	int		resolved;
}		t_dns_resp;

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
	t_dns_resp	dns_resp;
	t_dt	datetime;

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
	data->http_resp.content_type = NULL;
	data->http_resp.server = NULL;

	data->n_ping = 0;
	data->ping_i = 0;

	data->n_dns = 0;
	data->dns_i = 0;
	data->dns_resp.resolved = 0;
	data->dns_resp.ipv4 = NULL;
	data->dns_resp.ipv6 = NULL;
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

void	__curl(int i, int *pfd, t_data *data)
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

void	exec_child(int i, int *pfd, t_data *data)
{
	int		pid;

	pid = fork();
	if (!pid)
		__curl(i, pfd, data);
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

void	clean_http_log(int i, t_data *data)
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

void	http_log(int i, t_data *data)
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
	if (data->http_resp.content_type)
		ft_putstr_fd(data->http_resp.content_type, fd);
	else
		ft_putstr_fd("-", fd);
	ft_putstr_fd(" ", fd);
	if (data->http_resp.server)
		ft_putstr_fd(data->http_resp.server, fd);
	else
		ft_putendl_fd("- ", fd);
	close(fd);
}

void	disc_message(int i, t_data *data)
{
	int		pid;
	int		pfd[2];
	char	*args[12];
	char	*msg;

	msg = ft_strdup("{\"content\": \"ERROR (**");
	msg = ft_strjoins(msg, data->http_obj[i]->mon->name);
	msg = ft_strjoins(msg, "**)\"}");
	args[0] = "curl";
	args[1] = "-s";
	args[2] = "-X";
	args[3] = "POST";
	args[4] = "-H";
	args[5] = "Content-Type: application/json";
	args[6] = "-H";
	args[7] = "authorization: MTAwNDE0MjA0NDk0OTc5NDk3Nw.GrGrqK.2kAr9sM0esgCbKhwMOTdyuKIniMcQKGRokJV08",
	args[8] = "-d";
	args[9] = msg;
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
}
void	echo_http(int i, t_data *data)
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
	{
		printf("\x1b[38;5;9m%d\x1b[0m ", code);
		disc_message(i, data);
	}
	printf("%s\n", addr);
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
void	echo_dns(int i, t_data *data, int fail)
{
	char	*hour;
	char	*name;
	char	*addr;

	hour = data->datetime.fhour;
	name = data->dns_obj[i]->mon->name;
	addr = data->dns_obj[i]->mon->addr;
	printf("%s ", hour);
	printf("\x1b[38;5;8m%s\x1b[0m ", name);
	if (!fail)
		printf("%s \x1b[38;5;10m%s\x1b[0m\n", addr, "RESOLVED");
	else
		printf("%s \x1b[38;5;9m%s\x1b[0m\n", addr, "FAILED");
}
void	dns_log(int i, t_data *data, int fail)
{
	char	buff[9];
	int		fd;
	char	*name;
	char	*addr;
	char	*server;

	name = data->dns_obj[i]->mon->name;
	addr = data->dns_obj[i]->mon->addr;
	server = data->dns_obj[i]->domain;
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

	ft_putstr_fd(name, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(addr, fd);
	if (!fail)
	{
		ft_putstr_fd(" OK ", fd);
		ft_putstr_fd(data->dns_resp.ipv4, fd);
		ft_putstr_fd(" ", fd);
		if (data->dns_resp.ipv6)
			ft_putstr_fd(data->dns_resp.ipv6, fd);
		else
			ft_putstr_fd("-", fd);
		ft_putstr_fd("\n", fd);
	}
	else
	{
		ft_putstr_fd(" FAIL ", fd);
		ft_putstr_fd("(Host ", fd);
		ft_putstr_fd(addr, fd);
		ft_putstr_fd(" not found in server ", fd);
		ft_putstr_fd(server, fd);
		ft_putendl_fd(":53)", fd);
	}
	close(fd);
}
void	clean_dns_log(int i, t_data *data, int fail)
{
	char	*hour;
	char	*name;
	char	*addr;
	char	*ipv4;
	char	*ipv6;
	int		fd;

	hour = data->datetime.hour;
	name = data->dns_obj[i]->mon->name;
	addr = data->dns_obj[i]->mon->addr;
	ipv4 = data->dns_resp.ipv4;
	ipv6 = data->dns_resp.ipv6;
	fd = open("monitoring.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	ft_putstr_fd(hour, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(name, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(addr, fd);
	if (!fail)
		ft_putendl_fd(" OK", fd);
	else
		ft_putendl_fd(" FAIL", fd);
	close(fd);
}
char	*get_ip(char *line)
{
	char	*buff;
	char	*ip;

	buff = ft_strnstr(line, "address", ft_strlen(line));
	ip = ft_strtrim(buff + 8, " \n\t");
	return (ip);
}
int	dns_is_present(char *line, const char *param)
{
	if (!ft_strncmp(param, "has address", ft_strlen(param)))
		if (ft_strnstr(line, "has address", ft_strlen(line)))
			return (1);
	if (!ft_strncmp(param, "has IPv6", ft_strlen(param)))
		if (ft_strnstr(line, "has IPv6", ft_strlen(line)))
			return (1);
	return (0);
}
void	__host(int i, int *pfd, t_data *data)
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
void	dns_exec_child(int i, int *pfd, t_data *data)
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
		dns_exec_child(i, pfd, &data);
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
					if (dns_is_present(line, "has address"))
					{
						data.dns_resp.ipv4 = get_ip(line);
						data.dns_resp.resolved = 1;
					}
					if (dns_is_present(line, "has IPv6"))
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
			monitore_http(data, i);
	}
	wait(NULL);

	// int		pid[data.n_dns];
	// i = -1;
	// while (++i < data.n_dns)
	// {
	// 	pid[i] = fork();
	// 	if (pid[i] == 0)
	// 		monitore_dns(data, i);
	// }
	// wait(NULL);
	




	return (0);
}
