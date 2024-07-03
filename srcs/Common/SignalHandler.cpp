/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 17:43:30 by craimond          #+#    #+#             */
/*   Updated: 2024/07/03 18:10:23 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SignalHandler.hpp"
#include "system_calls.hpp"

volatile sig_atomic_t g_received_signal = 0;

SignalHandler::SignalHandler(void)
{
	_sa.sa_handler = &SignalHandler::handleSignal;
	_sa.sa_flags = 0;
	sigemptyset(&_sa.sa_mask);

	sigaction_p(SIGINT, &_sa, NULL);
	sigaction_p(SIGPIPE, &_sa, NULL);
}

SignalHandler::~SignalHandler(void) {}

inline void SignalHandler::handleSignal(int sig)
{
	g_received_signal = sig;
}

