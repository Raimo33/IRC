/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 12:27:17 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTHANDLER_HPP
# define EVENTHANDLER_HPP

# include <string>

using namespace std;

class Client;

class EventHandler
{
	public:
		EventHandler();
		EventHandler(const EventHandler &copy);
		~EventHandler();
		EventHandler	&operator=(const EventHandler &rhs);
		void processInput(string raw_input, const Client &client);
	private:
		t_input parseInput(string &raw_input) const;
		void executeCommandMode(const t_input &input, const Client &client);
		void executeCommandJoin(const t_input &input, const Client &client);
		void executeCommandNick(const t_input &input, const Client &client);
		void executeCommandPass(const t_input &input, const Client &client);
		void executeCommandPrivmsg(const t_input &input, const Client &client);
		void executeCommandQuit(const t_input &input, const Client &client);
		void executeCommandUser(const t_input &input, const Client &client);
};

#endif