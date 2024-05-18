/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/18 13:05:50 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <string>
# include <vector>

# include "Client.hpp"
# include "utils.hpp"

using namespace std;

//esiste un event handler per ogni client, che si occupa di gestire i comandi ricevuti dal client
class EventHandler
{
	public:
		explicit EventHandler(Client *client, Server *server);
		~EventHandler(void);

		void 						processInput(string raw_input);
		void						deliverMessage(const User &receiver, const PrivateMessage &message) const;
		void						deliverMessage(const Channel &receiver, const Message &message) const;

		class						CommandNotFoundException; //processInput, parseInput
		class						CantSendMessageToYourselfException; //deliverMessage

	private:
		typedef enum e_cmd {PRIVMSG, JOIN, MODE, PASS, NICK, USER, QUIT}				t_cmd;
		typedef struct s_input {string prefix; t_cmd command; vector<string> params;}	t_input;

		t_input						parseInput(string &raw_input) const;
		void						executeCommandPrivmsg(const vector<string> &params);
		void						executeCommandMode(const vector<string> &params);
		void						executeCommandJoin(const vector<string> &params);
		void						executeCommandPass(const vector<string> &params);
		void						executeCommandNick(const vector<string> &params);
		void						executeCommandQuit(const vector<string> &params);
		void						executeCommandUser(const vector<string> &params);
		void						sendBufferedString(const User &receiver, const string &string) const;

		const map<string, t_cmd>	_commands;
		const Client				*_client;
		const Server				*_server;
};

#endif