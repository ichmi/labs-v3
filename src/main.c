/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 14:15:46 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/08/02 23:25:43 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webmon.h"

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	init_data(&data);
	if (av[1])
		if (!ft_strncmp(av[1], "--simplify", 11))
			data.f_simplify = 1;
	init_lists(&data);
	tokenize_objs(&data);

	// int		pid[data.n_http];
	// i = -1;
	// while (++i < data.n_http)
	// {
	// 	pid[i] = fork();
	// 	if (pid[i] == 0)
	// 		monitore_http(data, i);
	// }
	// wait(NULL);

	int		pid[data.n_dns];
	i = -1;
	while (++i < data.n_dns)
	{
		pid[i] = fork();
		if (pid[i] == 0)
			monitore_dns(data, i);
	}
	wait(NULL);
	




	return (0);
}
