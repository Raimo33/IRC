/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_calls.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:47:21 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 17:48:53 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEMCALLS_HPP
#define SYSTEMCALLS_HPP

#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

int		bind_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int		listen_p(int sockfd, int backlog);
int		accept_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int		connect_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int		socket_p(int domain, int type, int protocol);
int		close_p(int fd);
int		shutdown_p(int sockfd, int how);
ssize_t send_p(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv_p(int sockfd, void *buf, size_t len, int flags);
int		getsockname_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int		setsockopt_p(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
int		getsockopt_p(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
int		fcntl_p(int fd, int cmd, ...);
int		epoll_create1_p(int flags);
int		epoll_ctl_p(int epfd, int op, int fd, struct epoll_event *event);
int		epoll_wait_p(int epfd, struct epoll_event *events, int maxevents, int timeout);
int		getaddrinfo_p(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
int		poll_p(struct pollfd *fds, nfds_t nfds, int timeout);

#endif