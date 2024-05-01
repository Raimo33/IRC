/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:19:11 by craimond          #+#    #+#             */
/*   Updated: 2024/05/01 18:55:21 by craimond         ###   ########.fr       */
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

# include "Channel.hpp"

using namespace std;

class Server
{
	public:
		
	private:
		vector<uint8_t>	_clients;
		vector<Channel>	_channels;
		vector<pollfd>	_pollfds;
		
}

#endif