/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 17:42:50 by craimond          #+#    #+#             */
/*   Updated: 2024/07/03 18:04:20 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALHANDLER_HPP
# define SIGNALHANDLER_HPP

# include <csignal>

extern volatile sig_atomic_t g_received_signal;

class SignalHandler
{
	public:
		SignalHandler(void);
		~SignalHandler(void);

	private:
		static void handleSignal(int sig);

		struct sigaction _sa;
};

#endif