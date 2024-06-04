/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BeetleBotActions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:41:12 by egualand          #+#    #+#             */
/*   Updated: 2024/06/04 14:08:27 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BEETLE_BOT_ACTIONS_HPP
# define BEETLE_BOT_ACTIONS_HPP

# include "AAction.hpp"

class ABot;

class SendLinkAction : public AAction
{
	public:
		explicit SendLinkAction(const ABot &owner);
		~SendLinkAction(void);

		const AMessage	*beExecuted(const CommandMessage &msg);
};

#endif