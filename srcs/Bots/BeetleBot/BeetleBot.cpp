/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BeetleBot.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 19:09:13 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 15:48:37 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BeetleBot.hpp"
#include "BeetleBotActions.hpp"

using std::string;
using std::map;

BeetleBot::BeetleBot(const string &nickname, const string &username, const string &realname) :
	ABot(nickname, username, realname) {}

BeetleBot::BeetleBot(const BeetleBot &copy) : ABot(copy) {}

BeetleBot::~BeetleBot(void) {}

const std::map<std::string, AAction *>	BeetleBot::initActions(void) const
{
	map<string, AAction *> actions;

	actions["BEET"] = new SendLinkAction();
	return actions;
}
