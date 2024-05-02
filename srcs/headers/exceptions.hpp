/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:26:36 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 11:29:39 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
# define EXCEPTIONS_HPP

#include <exception>
#include <string>

class GeneralException : public exception
{
	public:
		GeneralException(const string &msg) : _msg(msg) {}
		virtual const char	*what() const throw()
		{
			return _msg.c_str();
		}
	private:
		string	_msg;
};

class NotExistingModeException : public GeneralException
{
	public:
		NotExistingModeException() : GeneralException("Invalid channel mode") {}
};

class NotOperatorException : public GeneralException
{
	public:
		NotOperatorException() : GeneralException("User is not an operator in this channel") {}
};

#endif