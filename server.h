/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 17:19:19 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/07/28 18:11:05 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef server_h
# define server_h

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

typedef struct	s_server
{
	int		domain;
	int		service;
	int		protocol;
	u_long	interface;
	int		port;
	int		backlog;
	struct sockaddr_in	address;
	int		socket;
	void	(*launch)(struct s_server *);
}	t_server;

t_server	init(int, int, int, u_long, int, int, void (*)(t_server *));

#endif