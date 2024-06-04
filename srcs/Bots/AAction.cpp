/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AAction.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:36:13 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 15:49:15 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AAction.hpp"
#include "ABot.hpp"

using std::string;

AAction::AAction(const std::string &trigger_command) :
    _trigger_command(trigger_command) {}

AAction::~AAction(void) {}

const std::string &AAction::getTriggerCommand(void) const { return _trigger_command; }