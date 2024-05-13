/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:04:16 by craimond          #+#    #+#             */
/*   Updated: 2024/05/13 13:08:51 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/irc.hpp"

//primo argv e' la porta, il secondo e' la password
int main(void)
{
	try
	{
		Server	server;
		
		server.run();
		
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}