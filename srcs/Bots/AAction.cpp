/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AAction.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 17:36:13 by craimond          #+#    #+#             */
/*   Updated: 2024/06/03 18:03:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AAction.hpp"

const enum e_commands	&AAction::getTriggerCommand(void) const { return _trigger_command; }