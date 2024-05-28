/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SystemCalls.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:47:21 by craimond          #+#    #+#             */
/*   Updated: 2024/05/28 13:14:24 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEMCALLS_HPP
# define SYSTEMCALLS_HPP

# include <sys/socket.h>
# include <stdexcept>
# include <cstring>
# include <errno.h>
# include <unistd.h>
# include <poll.h>
# include <netdb.h>

int				bind_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int				listen_p(int sockfd, int backlog);
int				accept_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int				connect_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int				socket_p(int domain, int type, int protocol);
int				close_p(int fd);
int				poll_p(struct pollfd *fds, nfds_t nfds, int timeout);
int				shutdown_p(int sockfd, int how);
ssize_t			send_p(int sockfd, const void *buf, size_t len, int flags);
ssize_t			recv_p(int sockfd, void *buf, size_t len, int flags);
int				gethostname_p(char *name, size_t len);
struct hostent	*gethostbyname_p(const char *name);

#endif