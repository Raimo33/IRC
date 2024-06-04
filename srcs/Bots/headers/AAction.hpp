/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AAction.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:34:21 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 14:07:18 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_HPP
# define ACTION_HPP

#include "AMessage.hpp"
#include "CommandMessage.hpp"

#include <string>

class AAction
{
	public:
		explicit AAction(const std::string &trigger_command, const ABot &owner);
		virtual ~AAction(void) = 0;

		virtual const AMessage	*beExecuted(const CommandMessage &msg) = 0;  //TODO deve ritornare un messaggio malloccato da inviare al server
		const std::string		&getTriggerCommand(void) const;
		
	private:
		const std::string		_trigger_command;
		const ABot				&_owner;
};

#endif