/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:19:11 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 11:30:21 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

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
# include <iostream>
# include <vector>
# include <unordered_map>

# include "Server.hpp"
# include "User.hpp"
# include "Channel.hpp"
# include "ChannelOperator.hpp"

#endif