/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AAction.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:34:21 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 15:45:34 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_HPP
#define ACTION_HPP

#include "AMessage.hpp"
#include "CommandMessage.hpp"

#include <string>

class ABot;

class AAction
{
  public:
	explicit AAction(const std::string &trigger_command);
	virtual ~AAction(void) = 0;

	virtual const AMessage *beExecuted(const CommandMessage &msg, const ABot &executor) const = 0;
	const std::string	   &getTriggerCommand(void) const;

  private:
	const std::string _trigger_command;
};

#endif