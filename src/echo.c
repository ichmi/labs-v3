/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 21:05:11 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/02 23:02:29 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webmon.h"

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
		http_disc_message(i, data);
	}
	printf("%s\n", addr);
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
	{
		printf("%s \x1b[38;5;9m%s\x1b[0m\n", addr, "FAILED");
		dns_disc_message(i, data);
	}
}
