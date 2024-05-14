/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:42:44 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 11:48:06 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/wrapper_functions.hpp"

int	bind_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	if (bind(sockfd, addr, addrlen) == -1)
	{
		perror("bind");
		throw std::runtime_error("Failed to bind socket");
	}
	return (0);
}

int	listen_p(int sockfd, int backlog)
{
	if (listen(sockfd, backlog) == -1)
	{
		perror("listen");
		throw std::runtime_error("Failed to listen on socket");
	}
	return (0);
}

int	accept_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int	new_sockfd;

	new_sockfd = accept(sockfd, addr, addrlen);
	if (new_sockfd == -1)
	{
		perror("accept");
		throw std::runtime_error("Failed to accept connection");
	}
	return (new_sockfd);
}

