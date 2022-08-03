/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 21:03:11 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/03 15:52:39 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webmon.h"

static void	wr_common_info(int i, int fd, t_data *data)
{
	char	*day_week;
	char	*day;
	char	*month;
	char	*year;
	char	*hour;

	day_week = data->datetime.day_week;
	day = data->datetime.day;
	month = data->datetime.month;
	year = data->datetime.year;
	hour = data->datetime.hour;
	ft_putstr_fd(day_week, fd); ft_putstr_fd(" ", fd);
	ft_putstr_fd(day, fd); ft_putstr_fd("-", fd);
	ft_putstr_fd(month, fd); ft_putstr_fd("-", fd);
	ft_putstr_fd(year, fd); ft_putstr_fd(" ", fd);
	ft_putstr_fd(hour, fd);
	ft_putstr_fd(" ", fd);
}

static void	wr_http_info(int i, int fd, t_data *data)
{
	char	*id;
	int		expected_code;
	int		received_code;
	char	*protocol;
	char	*method;
	char	*addr;
	char	*content_type;
	char	*server;

	id = data->http_obj[i]->mon->name;
	expected_code = data->http_obj[i]->expected_status_code;
	received_code = data->http_resp.code;
	protocol = data->http_resp.prot;
	method = data->http_obj[i]->method;
	addr = data->http_obj[i]->mon->addr;
	content_type = data->http_resp.content_type;
	server = data->http_resp.server;
	ft_putstr_fd(id, fd); ft_putstr_fd(" ", fd);
	ft_putnbr_fd(expected_code, fd); ft_putstr_fd(":", fd);
	ft_putnbr_fd(received_code, fd); ft_putstr_fd(" ", fd);
	ft_putstr_fd(protocol, fd); ft_putstr_fd(" ", fd);
	ft_putstr_fd(method, fd); ft_putstr_fd(" ", fd);
	ft_putstr_fd(addr, fd); ft_putstr_fd(" ", fd);
	if (content_type && server)
	{
		ft_putstr_fd(content_type, fd); ft_putstr_fd(" ", fd);
		ft_putstr_fd(server, fd);
		ft_putstr_fd("\n", fd);
	}
	else
	{
		if (content_type)
			ft_putstr_fd(content_type, fd);
		else
			ft_putstr_fd("-", fd);
		ft_putstr_fd(" ", fd);
		if (server)
			ft_putstr_fd(server, fd);
		else
			ft_putendl_fd("-", fd);
	}
}

void	http_log(int i, t_data *data)
{
	char	buff[9];
	int		fd;

	fd = open("monitoring.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	wr_common_info(i, fd, data);
	wr_http_info(i, fd, data);
	close(fd);
}

void	clean_http_log(int i, t_data *data)
{
	char	*hour;
	char	*id;
	int		expected_code;
	int		received_code;
	char	*addr;
	int		fd;

	hour = data->datetime.hour;
	id = data->http_obj[i]->mon->name;
	expected_code = data->http_obj[i]->expected_status_code;
	received_code = data->http_resp.code;
	addr = data->http_obj[i]->mon->addr;
	fd = open("monitoring.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	ft_putstr_fd(hour, fd); ft_putstr_fd(" ", fd);
	ft_putstr_fd(id, fd); ft_putstr_fd(" ", fd);
	ft_putnbr_fd(expected_code, fd); ft_putstr_fd(":", fd);
	ft_putnbr_fd(received_code, fd); ft_putstr_fd(" ", fd);
	ft_putendl_fd(addr, fd);
	close(fd);
}

void	dns_log(int i, t_data *data, int fail)
{
	char	buff[9];
	int		fd;
	char	*id;
	char	*addr;
	char	*server;

	id = data->dns_obj[i]->mon->name;
	addr = data->dns_obj[i]->mon->addr;
	server = data->dns_obj[i]->domain;
	fd = open("monitoring.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	wr_common_info(i, fd, data);

	ft_putstr_fd(id, fd);
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
		dns_disc_message(i, data);
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

void	ping_log(int i, t_data *data, int fail)
{
	char	*hour;
	char	*id;
	char	*addr;
	char	*ipv4;
	int		rtt;
	int		fd;

	hour = data->datetime.hour;
	id = data->ping_obj[i]->mon->name;
	addr = data->ping_obj[i]->mon->addr;
	ipv4 = data->ping_resp.ipv4;
	rtt = data->ping_resp.rtt;

	fd = open("monitoring.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	wr_common_info(i, fd, data);
	ft_putstr_fd(id, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(ipv4, fd);
	if (!fail)
	{
		ft_putstr_fd(" OK ", fd);
		ft_putstr_fd(ipv4, fd);
		ft_putstr_fd(" ", fd);
		ft_putnbr_fd(rtt, fd);
		ft_putstr_fd("\n", fd);
	}
	else
	{
		ft_putendl_fd(" ICMP FAILED", fd);
		dns_disc_message(i, data);
	}
	close(fd);
}

void	clean_ping_log(int i, t_data *data, int fail)
{
	char	*hour;
	char	*name;
	char	*addr;
	int		fd;

	hour = data->datetime.hour;
	name = data->ping_obj[i]->mon->name;
	addr = data->ping_obj[i]->mon->addr;
	fd = open("monitoring.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	ft_putstr_fd(hour, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(name, fd);
	ft_putstr_fd(" ", fd);
	ft_putstr_fd(addr, fd);
	if (!fail)
		ft_putendl_fd(" ICMP OK", fd);
	else
		ft_putendl_fd(" ICMP FAILED", fd);
	close(fd);
}
