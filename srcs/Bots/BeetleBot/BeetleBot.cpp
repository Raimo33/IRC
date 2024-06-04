/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BeetleBot.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 19:09:13 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 18:59:34 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BeetleBot.hpp"
#include "BeetleBotActions.hpp"

using std::map;
using std::string;

BeetleBot::BeetleBot(const string &nickname, const string &username, const string &realname) :
    ABot(nickname, username, realname)
{
	_actions = initActions();
}

BeetleBot::BeetleBot(const BeetleBot &copy) :
    ABot(copy) {}

BeetleBot::~BeetleBot(void) {}

std::map<std::string, AAction *> BeetleBot::initActions(void) const
{
	map<string, AAction *> actions;

	actions["!JUICE"] = new SendLinkAction();
	return actions;
}
