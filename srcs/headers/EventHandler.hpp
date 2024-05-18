/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 16:43:25 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <string>
# include <vector>

# include "Client.hpp"
# include "utils.hpp"

using namespace std;

typedef enum e_cmd
{
	CMD_PASS = 0,
	CMD_NICK = 1,
	CMD_USER = 2,
	CMD_JOIN = 3,
	CMD_PRIVMSG = 4,
	CMD_MODE = 5,
	CMD_QUIT = 6
}	t_cmd;

typedef struct s_input
{
	string			prefix;
	t_cmd			command;
	vector<string>	params;
}	t_input;

//esiste un event handler per ogni client, che si occupa di gestire i comandi ricevuti dal client
class EventHandler
{
	public:
		explicit EventHandler(Client *client, Server *server);
		~EventHandler(void);

		const map<string, t_cmd>	getCommands(void) const;
		const Client					*getClient(void) const;
		const Server					*getServer(void) const;

		void 							processInput(string raw_input);
		void							deliverMessage(const User &receiver, const PrivateMessage &message) const;
		void							deliverMessage(const Channel &receiver, const Message &message) const;

		class							CommandNotFoundException; //processInput, parseInput
		class							CantSendMessageToYourselfException; //deliverMessage

	private:

		t_input							parseInput(string &raw_input) const;
		void							executeCommandPrivmsg(const vector<string> &params);
		void							executeCommandMode(const vector<string> &params);
		void							executeCommandJoin(const vector<string> &params);
		void							executeCommandPass(const vector<string> &params);
		void							executeCommandNick(const vector<string> &params);
		void							executeCommandQuit(const vector<string> &params);
		void							executeCommandUser(const vector<string> &params);
		void							sendBufferedString(const User &receiver, const string &string) const;

		const map<string, t_cmd>		_commands;
		const Client					*_client;
		const Server					*_server;
};

#endif