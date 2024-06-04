/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BeetleBot.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 19:09:13 by craimond          #+#    #+#             */
/*   Updated: 2024/06/03 17:09:25 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BeetleBot.hpp"

using std::string;

BeetleBot::BeetleBot(const string &nickname, const string &username, const string &realname) :
	ABot(nickname, username, realname) {}

BeetleBot::BeetleBot(const BeetleBot &copy) : ABot(copy) {}

BeetleBot::~BeetleBot(void) {}

void	BeetleBot::routine(void)
{
	//TODO implement the bot's routine
}