/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 15:17:38 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <string>
# include <vector>

# include "Client.hpp"
# include "utils.hpp"

using namespace std;

enum e_cmd_type
{
	PASS = 0,
	NICK = 1,
	USER = 2,
	JOIN = 3,
	PRIVMSG = 4,
	MODE = 5,
	QUIT = 6
};

struct s_input
{
	string			prefix;
	e_cmd_type		command;
	vector<string>	params;
};

//esiste un event handler per ogni client, che si occupa di gestire i comandi ricevuti dal client
class EventHandler
{
	public:
		explicit EventHandler(Client *client, Server *server);
		~EventHandler(void);

		const map<string, e_cmd_type>	&getCommands(void) const;
		const Client					*getClient(void) const;
		const Server					*getServer(void) const;

		void 							processInput(string raw_input);
		void							deliverMessage(const User &receiver, const PrivateMessage &message) const;
		void							deliverMessage(const Channel &receiver, const Message &message) const;

		class							CommandNotFoundException; //processInput, parseInput
		class							CantSendMessageToYourselfException; //deliverMessage

	private:

		s_input							parseInput(string &raw_input) const;
		void							executeCommandPrivmsg(const vector<string> &params);
		void							executeCommandMode(const vector<string> &params);
		void							executeCommandJoin(const vector<string> &params);
		void							executeCommandPass(const vector<string> &params);
		void							executeCommandNick(const vector<string> &params);
		void							executeCommandQuit(const vector<string> &params);
		void							executeCommandUser(const vector<string> &params);
		void							sendBufferedString(const User &receiver, const string &string) const;
		const map<string, e_cmd_type>	&initCommandMap(void) const;

		const map<string, e_cmd_type>	_commands;
		const Client					*_client;
		const Server					*_server;
};

class EventHandler::CommandNotFoundException : public exception
{
	public:
		const char	*what(void) const throw();
};

class EventHandler::CantSendMessageToYourselfException : public exception
{
	public:
		const char	*what(void) const throw();
};

#endif