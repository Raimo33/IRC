/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BeetleBotActions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:41:12 by egualand          #+#    #+#             */
/*   Updated: 2024/06/05 15:51:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BEETLE_BOT_ACTIONS_HPP
#define BEETLE_BOT_ACTIONS_HPP

#include "AAction.hpp"

class ABot;

class SendLinkAction : public AAction
{
  public:
	explicit SendLinkAction(void);
	~SendLinkAction(void);

	const AMessage *beExecuted(const CommandMessage &msg, const ABot &executor) const;
};

#endif