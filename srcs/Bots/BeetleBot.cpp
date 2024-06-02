/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BeetleBot.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 19:09:13 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 22:40:38 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BeetleBot.hpp"

BeetleBot::BeetleBot(const std::string &nickname, const std::string &username, const std::string &realname) :
	ABot(nickname, username, realname) {}

BeetleBot::BeetleBot(const BeetleBot &copy) : ABot(copy) {}

BeetleBot::~BeetleBot(void) {}

void	BeetleBot::routine(void)
{
	//TODO implement the bot's routine
}