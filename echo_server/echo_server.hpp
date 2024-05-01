/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_server.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:43:43 by craimond          #+#    #+#             */
/*   Updated: 2024/05/01 22:56:11 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ECHO_SERVER_HPP
# define ECHO_SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/select.h>
# include <sys/time.h>
# include <poll.h>
# include <sys/epoll.h>
# include <cstring>
# include <cstdlib>
# include <iostream>
# include <cstdio>
# include <unistd.h>

using namespace std;

# define BUFFER_SIZE 1024

#endif