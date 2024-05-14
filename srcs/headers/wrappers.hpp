/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:47:21 by craimond          #+#    #+#             */
/*   Updated: 2024/05/14 16:33:16 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRAPPERS_HPP
# define WRAPPERS_HPP

int	bind_p(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int	listen_p(int sockfd, int backlog);
int	accept_p(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

#endif