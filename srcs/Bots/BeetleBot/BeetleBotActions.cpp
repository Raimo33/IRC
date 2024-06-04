/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BeetleBotActions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:42:53 by egualand          #+#    #+#             */
/*   Updated: 2024/06/04 19:21:27 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BeetleBotActions.hpp"
#include "ABot.hpp"

using std::string;

SendLinkAction::SendLinkAction(void) :
    AAction("!JUICE") {}

SendLinkAction::~SendLinkAction(void) {}

const AMessage *SendLinkAction::beExecuted(const CommandMessage &msg, const ABot &executor) const
{
	const string &destination = msg.getParams().at(0);
	const char   *link        = "https://shorturl.at/YxRoE";
	return new CommandMessage(executor.getNickname(), PRIVMSG, destination.c_str(), link);
}
