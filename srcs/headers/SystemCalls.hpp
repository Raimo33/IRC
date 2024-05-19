/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SystemCalls.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:47:21 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 14:59:41 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRAPPERS_HPP
# define WRAPPERS_HPP

# include <sys/socket.h>
# include <stdexcept>
# include <cstring>
# include <errno.h>
# include <unistd.h>
# include <poll.h>

using namespace std;

int	bind_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int	listen_p(int sockfd, int backlog);
int	accept_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int	connect_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int	socket_p(int domain, int type, int protocol);
int	close_p(int fd);
int poll_p(struct pollfd *fds, nfds_t nfds, int timeout);
int shutdown_p(int sockfd, int how);
ssize_t send_p(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv_p(int sockfd, void *buf, size_t len, int flags);

class SystemErrorException : public runtime_error
{
	public:
		explicit SystemErrorException(const string &msg);
};

#endif