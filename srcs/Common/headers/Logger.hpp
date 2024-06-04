/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/06/04 20:12:29 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

const char *const RED = "\033[0;31m";
const char *const BLUE = "\033[0;34m";
const char *const ORANGE = "\033[0;33m";
const char *const YELLOW = "\033[0;33m";
const char *const RESET = "\033[0m";

class Logger
{
  public:
	Logger(void);
	explicit Logger(const std::string &filename);
	Logger(const Logger &copy);
	~Logger(void);

	Logger &operator=(const Logger &rhs);

	const std::string &getFilename(void) const;
	void               setFile(const std::string &filename);

	void logEvent(const std::string &message) const;
	void logError(const std::exception *e) const;

  private:
	void              log(const std::string &message, const char *const color = "") const;
	void              logToFile(const std::string &timestamp, const std::string &message) const;
	void              logToConsole(const std::string &timestamp, const std::string &message, const char *const color = "") const;
	const std::string getTimestamp(void) const;

	std::string           _filename;
	mutable std::ofstream _file;
};

#endif