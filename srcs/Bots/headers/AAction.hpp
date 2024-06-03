/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AAction.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:34:21 by craimond          #+#    #+#             */
/*   Updated: 2024/06/03 18:19:59 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_HPP
# define ACTION_HPP

#include "AMessage.hpp"
#include "CommandMessage.hpp"

class AAction
{
	public:
		explicit AAction(const enum e_commands &trigger_command);
		virtual ~AAction(void) = 0;

		virtual const AMessage	*beExecuted(const CommandMessage &msg) = 0;  //TODO deve ritornare un messaggio malloccato da inviare al server
		const enum e_commands	&getTriggerCommand(void) const;
		
	private:
		const enum e_commands	_trigger_command;
};

#endif