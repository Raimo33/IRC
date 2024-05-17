/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/17 17:50:17 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <string>
# include <vector>

# include "Client.hpp"
# include "utils.hpp"

using namespace std;

class EventHandler
{
	public:
		EventHandler(void);
		EventHandler(Client *client, Server *server);
		EventHandler(const EventHandler &copy);
		~EventHandler(void);
		EventHandler	&operator=(const EventHandler &rhs);
		void 	processInput(string raw_input);
		void 	setClient(Client *client);
		void 	setServer(Server *server);
		Client	&getClient(void) const;
		Server	&getServer(void) const;
		void	deliverMessage(const User &receiver, const PrivateMessage &message) const;
		void	deliverMessage(const Channel &receiver, const Message &message) const;
		class		UnknownCommandException;
	private:
		t_input parseInput(string &raw_input) const;
		void executeCommandPrivmsg(const vector<string> &params);
		void executeCommandMode(const vector<string> &params);
		void executeCommandJoin(const vector<string> &params);
		void executeCommandPass(const vector<string> &params);
		void executeCommandNick(const vector<string> &params);
		void executeCommandQuit(const vector<string> &params);
		void executeCommandUser(const vector<string> &params);
		void sendBufferedString(const User &receiver, const string &string) const;
		map<string, t_cmd>	_commands;
		Client				*_client;
		Server				*_server;
};

class EventHandler::UnknownCommandException : public exception
{
	public:
		virtual const char *what() const throw();
};

#endif