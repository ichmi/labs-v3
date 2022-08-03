/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webmon.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:51:31 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/03 14:47:46 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBMON_H
# define WEBMON_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <pthread.h>

# include <ctype.h>
# include <time.h>

# include <netdb.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# include "../libft/libft.h"

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

typedef struct s_ping_resp
{
	char	*ipv4;
	int		rtt;
}		t_ping_resp;

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
	t_http_resp	http_resp;
	t_http		**http_obj;
	char		**http_list;
	int			n_http;
	int			http_i;

	t_ping		**ping_obj;
	char		**ping_list;
	int			n_ping;
	int			ping_i;
	t_ping_resp	ping_resp;

	t_dns_resp	dns_resp;
	t_dns		**dns_obj;
	char		**dns_list;
	int			n_dns;
	int			dns_i;

	t_dt		datetime;
	int			n_mon;
	int			f_simplify;
}		t_data ;


// init/constructors
void	init_data(t_data *data);
void	init_lists(t_data *data);
void	tokenize_objs(t_data *data);

// http
void	monitore_http(t_data data, int i);
void	monitore_ping(t_data data, int i);
void	monitore_dns(t_data data, int i);

// utils
int		is_present(char *line, const char *param);
void	http_disc_message(int i, t_data *data);
void	dns_disc_message(int i, t_data *data);
void	set_datetime(t_data *data);

// log
void	http_log(int i, t_data *data);
void	clean_http_log(int i, t_data *data);
void	dns_log(int i, t_data *data, int fail);
void	clean_dns_log(int i, t_data *data, int fail);
void	ping_log(int i, t_data *data, int fail);
void	clean_ping_log(int i, t_data *data, int fail);

// echo
void	echo_http(int i, t_data *data);
void	echo_dns(int i, t_data *data, int fail);
void	echo_ping(int i, t_data *data, int fail);

#endif
