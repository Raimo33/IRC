/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper_functions.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:47:21 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 11:47:49 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRAPPER_FUNCTIONS_HPP
# define WRAPPER_FUNCTIONS_HPP

# include "irc.hpp"

int	bind_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int	listen_p(int sockfd, int backlog);
int	accept_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

#endif