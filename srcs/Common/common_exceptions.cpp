/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common_exceptions.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:27:57 by craimond          #+#    #+#             */
/*   Updated: 2024/06/02 17:12:11 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <map>
#include <vector>
#include <string>
#include <errno.h>
#include <cstring>

#include "common_exceptions.hpp"

using std::string;
using std::map;
using std::vector;

SystemErrorException::SystemErrorException(const string &msg) : runtime_error(msg) {}

SystemErrorException::SystemErrorException(const int errnum) : runtime_error(strerror(errnum)) {}

const char *SystemErrorException::what(void) const throw()
{
	return runtime_error::what();
}

InternalErrorException::InternalErrorException(const string &msg) : runtime_error(msg) {}

const char *InternalErrorException::what(void) const throw()
{
	return runtime_error::what();
}