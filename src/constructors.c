/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:38:29 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/02 23:13:09 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webmon.h"

static t_http	*get_http_obj(char **tab)
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

static t_ping	*get_ping_obj(char **tab)
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

static t_dns	*get_dns_obj(char **tab)
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

void	tokenize_objs(t_data *data)
{
	char	**tab;
	int		i;

	i = -1;
	while (data->http_list[++i])
	{
		tab = ft_split(data->http_list[i], '\t');
		data->http_obj[i] = get_http_obj(tab);
		ft_free_matrix(tab);
	}
	i = -1;
	while (data->ping_list[++i])
	{
		tab = ft_split(data->ping_list[i], '\t');
		data->ping_obj[i] = get_ping_obj(tab);
		ft_free_matrix(tab);
	}
	i = -1;
	while (data->dns_list[++i])
	{
		tab = ft_split(data->dns_list[i], '\t');
		data->dns_obj[i] = get_dns_obj(tab);
		ft_free_matrix(tab);
	}
}
