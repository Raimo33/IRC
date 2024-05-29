/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SystemCalls.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:42:44 by craimond          #+#    #+#             */
/*   Updated: 2024/05/29 15:58:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "irc/Exceptions.hpp"
#include "irc/SystemCalls.hpp"

using std::string;

int	bind_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	if (bind(sockfd, addr, addrlen) == -1)
		throw SystemErrorException(errno);
	return (0);
}

int	listen_p(int sockfd, int backlog)
{
	if (listen(sockfd, backlog) == -1)
		throw SystemErrorException(errno);
	return (0);
}

int	accept_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int	new_sockfd;

	new_sockfd = accept(sockfd, addr, addrlen);
	if (new_sockfd == -1)
		throw SystemErrorException(errno);
	return (new_sockfd);
}

int	connect_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	if (connect(sockfd, addr, addrlen) == -1)
		throw SystemErrorException(errno);
	return (0);
}

int	socket_p(int domain, int type, int protocol)
{
	int	sockfd;

	sockfd = socket(domain, type, protocol);
	if (sockfd == -1)
		throw SystemErrorException(errno);
	return (sockfd);
}

int	close_p(int fd)
{
	int	ret;

	ret = close(fd);
	if (ret == -1)
		throw SystemErrorException(errno);
	return (ret);
}

int poll_p(struct pollfd *fds, nfds_t nfds, int timeout)
{
	int	ret;

	ret = poll(fds, nfds, timeout);
	if (ret == -1)
		throw SystemErrorException(errno);
	return (ret);
}

int	shutdown_p(int sockfd, int how)
{
	int	ret;

	ret = shutdown(sockfd, how);
	if (ret == -1)
		throw SystemErrorException(errno);
	return (ret);
}

ssize_t	send_p(int sockfd, const void *buf, size_t len, int flags)
{
	ssize_t	ret;

	ret = send(sockfd, buf, len, flags);
	if (ret == -1)
		throw SystemErrorException(errno);
	return (ret);
}

ssize_t	recv_p(int sockfd, void *buf, size_t len, int flags)
{
	ssize_t	ret;

	ret = recv(sockfd, buf, len, flags);
	if (ret == -1)
		throw SystemErrorException(errno);
	return (ret);
}

int	gethostname_p(char *name, size_t len)
{
	int	ret;

	ret = gethostname(name, len);
	if (ret == -1)
		throw SystemErrorException(errno);
	return (ret);
}

struct hostent	*gethostbyname_p(const char *name)
{
	hostent	*ret;

	ret = gethostbyname(name);
	if (!ret)
		throw SystemErrorException(errno);
	return (ret);
}

int	fcntl_p(int fd, int cmd, ...)
{
	va_list args;
	va_start(args, cmd);
	int result;

	switch (cmd) {
		case F_GETFL:
		case F_GETFD:
			result = fcntl(fd, cmd);
			break;
		case F_SETFL:
		case F_SETFD: {
			int arg = va_arg(args, int);
			result = fcntl(fd, cmd, arg);
			break;
		}
		case F_GETLK:
		case F_SETLK:
		case F_SETLKW: {
			struct flock* arg = va_arg(args, struct flock*);
			result = fcntl(fd, cmd, arg);
			break;
		}
		default:
			result = -1;
			errno = EINVAL;
			break;
	}

	va_end(args);

	if (result == -1) {
		throw SystemErrorException(errno);
	}

	return result;
}
