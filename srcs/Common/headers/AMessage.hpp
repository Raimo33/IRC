/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:38:24 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 13:34:36 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMESSAGE_HPP
#define AMESSAGE_HPP

#include <stdint.h>
#include <string>
#include <vector>
class AMessage
{
  public:
	AMessage(void);
	AMessage(const AMessage &copy);
	virtual ~AMessage(void) = 0;

	AMessage &operator=(const AMessage &copy);

	const std::string			   &getPrefix(void) const;
	void							setPrefix(const std::string &prefix);
	const std::vector<std::string> &getParams(void) const;
	void							setParams(const std::vector<std::string> &params);
	void							setParam(const std::string &param, const int32_t index = -1);

	const std::string getRaw(void) const;
	void			  getDelivered(const int socket) const;

  protected:
	virtual void parse(std::string raw_input) = 0;
	virtual void unwrapMessage(std::string &first_part, std::string &second_part) const = 0;

	std::string				 _prefix;
	std::vector<std::string> _params;
};

#endif