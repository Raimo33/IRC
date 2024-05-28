/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SystemCalls.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:42:44 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 12:49:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "irc/Exceptions.hpp"
#include "irc/SystemCalls.hpp"

using std::string;

int	bind_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	if (bind(sockfd, addr, addrlen) == -1)
		throw SystemErrorException("bind: " + string(strerror(errno)));
	return (0);
}

int	listen_p(int sockfd, int backlog)
{
	if (listen(sockfd, backlog) == -1)
		throw SystemErrorException("listen: " + string(strerror(errno)));
	return (0);
}

int	accept_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int	new_sockfd;

	new_sockfd = accept(sockfd, addr, addrlen);
	if (new_sockfd == -1)
		throw SystemErrorException("accept: " + string(strerror(errno)));
	return (new_sockfd);
}

int	connect_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	if (connect(sockfd, addr, addrlen) == -1)
		throw SystemErrorException("connect: " + string(strerror(errno)));
	return (0);
}

int	socket_p(int domain, int type, int protocol)
{
	int	sockfd;

	sockfd = socket(domain, type, protocol);
	if (sockfd == -1)
		throw SystemErrorException("socket: " + string(strerror(errno)));
	return (sockfd);
}

int	close_p(int fd)
{
	int	ret;

	ret = close(fd);
	if (ret == -1)
		throw SystemErrorException("close: " + string(strerror(errno)));
	return (ret);
}

int poll_p(struct pollfd *fds, nfds_t nfds, int timeout)
{
	int	ret;

	ret = poll(fds, nfds, timeout);
	if (ret == -1)
		throw SystemErrorException("poll: " + string(strerror(errno)));
	return (ret);
}

int	shutdown_p(int sockfd, int how)
{
	int	ret;

	ret = shutdown(sockfd, how);
	if (ret == -1)
		throw SystemErrorException("shutdown: " + string(strerror(errno)));
	return (ret);
}

ssize_t	send_p(int sockfd, const void *buf, size_t len, int flags)
{
	ssize_t	ret;

	ret = send(sockfd, buf, len, flags);
	if (ret == -1)
		throw SystemErrorException("send: " + string(strerror(errno)));
	return (ret);
}

ssize_t	recv_p(int sockfd, void *buf, size_t len, int flags)
{
	ssize_t	ret;

	ret = recv(sockfd, buf, len, flags);
	if (ret == -1)
		throw SystemErrorException("recv: " + string(strerror(errno)));
	return (ret);
}
