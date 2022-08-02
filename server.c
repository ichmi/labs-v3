/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 16:30:01 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/07/28 18:41:22 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

int	main()
{
	FILE	*body = fopen("index.html", "r");

	char	response[1024];
	fgets(response, 1024, body);

	char	header[2048] = "HTTP/1.1 200 OK\r\n\n";
	strcat(header, response);

	int		server_sck = socket(AF_INET, SOCK_STREAM, 0);
	struct	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(80);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	bind(server_sck, (struct sockaddr *) &server_addr, sizeof(server_addr));

	listen(server_sck, 10);

	int	client_sck;
	while (1)
	{
		client_sck = accept(server_sck, NULL, NULL);
		send(client_sck, header, sizeof(header), 0);
		close(client_sck);
	}
	return (0);
}
