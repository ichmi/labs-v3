/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   some.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 18:48:50 by frosa-ma          #+#    #+#             */
/*   Updated: 2022/07/31 16:38:21 by frosa-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>



int	main(int ac, char **av)
{
	// ac = 0;
	// char	*address = av[1];

	struct hostent	*remote_host;
	struct in_addr	*net_address;

	remote_host = gethostbyname(av[1]);
	net_address = (struct in_addr *)remote_host->h_addr;
	char	*str_address = inet_ntoa(*net_address);
	printf("%s\n", str_address);

	int		client_sck = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in	remote_addr;
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_port = htons(80);
	remote_addr.sin_addr = *net_address;
	// remote_addr.sin_addr.s_addr = net_address;
	// inet_aton(address, &remote_addr.sin_addr.s_addr);

	connect(client_sck, (struct sockaddr *) &remote_addr, sizeof(remote_addr));

	// char	request[] = "GET / HTTP/1.1\n\n";
	char	request[] = "GET / HTTP/1.1\r\n\r\n";
	char	response[1024];

	send(client_sck, request, sizeof(request), 0);
	recv(client_sck, &response, sizeof(response), 0);

	printf("Response from the server: %s\n", response);
	close(client_sck);

	return (0);
}

		int t;
		t = fork();
		char ipv4[20];
		if (t == 0)
		{
			struct hostent	*remote_host;
			struct in_addr	*net_address;
			char	*buff;

			remote_host = gethostbyname(data.http_obj[i]->mon->addr);
			net_address = (struct in_addr *)remote_host->h_addr;
			buff = inet_ntoa(*net_address);
			int x = open("teste", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			int siz = ft_strlen(buff);
			write(x, &siz, sizeof(int));
			write(x, buff, ft_strlen(buff) + 1);
			close(x);
		}
		wait(NULL);
		int x = open("teste", O_RDONLY);
		int siz = 0;
		read(x, &siz, sizeof(int));
		read(x, ipv4, siz + 1);
		close(x);