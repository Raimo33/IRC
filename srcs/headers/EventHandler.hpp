/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/19 15:45:04 by craimond         ###   ########.fr       */
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
	string					prefix;
	e_cmd_type				command;
	vector<const string>	params;
};

//esiste un event handler per ogni client, che si occupa di gestire i comandi ricevuti dal client
class EventHandler
{
	public:
		EventHandler(void);
		EventHandler(const EventHandler &copy);
		~EventHandler(void);

		const map<string, e_cmd_type>	&getCommands(void) const;
		const Client					*getClient(void) const;
		void							setClient(Client *client);

		void 							processInput(string raw_input);
		void							deliverMessage(const User &receiver, const PrivateMessage &message) const;
		void							deliverMessage(const Channel &receiver, const Message &message) const;

		class							CommandNotFoundException; //processInput, parseInput
		class							CantSendMessageToYourselfException; //deliverMessage

	private:

		s_input							parseInput(string &raw_input) const;
		void							executeCommandPrivmsg(const vector<const string> &params);
		void							executeCommandMode(const vector<const string> &params);
		void							executeCommandJoin(const vector<const string> &params);
		void							executeCommandPass(const vector<const string> &params);
		void							executeCommandNick(const vector<const string> &params);
		void							executeCommandQuit(const vector<const string> &params);
		void							executeCommandUser(const vector<const string> &params);
		void							sendBufferedString(const User &receiver, const string &string) const;
		const map<string, e_cmd_type>	&initCommandMap(void) const;

		const map<string, e_cmd_type>	_commands; //TODO spostare fuori (pseudo globale) altrimenti viene creato ad ogni evento per ogni client
		Client							*_client;
		Server							*_server;
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