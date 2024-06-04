/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BeetleBotActions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:42:53 by egualand          #+#    #+#             */
/*   Updated: 2024/06/04 14:18:54 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BeetleBotActions.hpp"

using std::string;

SendLinkAction::SendLinkAction(const ABot &owner) : AAction("JUICE", owner) {}

SendLinkAction::~SendLinkAction(void) {}

const AMessage	*SendLinkAction::beExecuted(const CommandMessage &msg)
{
	const string &destination = msg.getParams().at(0); //TODO controllare se funziona
	return CommandMessage(_owner.getNickname(), PRIVMSG,
}