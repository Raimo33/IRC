/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SystemCalls.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:47:21 by craimond          #+#    #+#             */
/*   Updated: 2024/05/31 18:23:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEMCALLS_HPP
# define SYSTEMCALLS_HPP

# include <sys/socket.h>
# include <stdexcept>
# include <cstring>
# include <errno.h>
# include <unistd.h>
# include <sys/epoll.h>
# include <netdb.h>
# include <fcntl.h>


int				bind_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int				listen_p(int sockfd, int backlog);
int				accept_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int				connect_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int				socket_p(int domain, int type, int protocol);
int				close_p(int fd);
int				shutdown_p(int sockfd, int how);
ssize_t			send_p(int sockfd, const void *buf, size_t len, int flags);
ssize_t			recv_p(int sockfd, void *buf, size_t len, int flags);
int				getsockname_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int				setsockopt_p(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
int				fcntl_p(int fd, int cmd, ...);
int				epoll_create1_p(int flags);
int				epoll_ctl_p(int epfd, int op, int fd, struct epoll_event *event);
int				epoll_wait_p(int epfd, struct epoll_event *events, int maxevents, int timeout);

#endif