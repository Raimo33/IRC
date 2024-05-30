/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:16:50 by craimond          #+#    #+#             */
/*   Updated: 2024/05/30 20:22:30 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/Logger.hpp"
#include "irc/Exceptions.hpp"

# include <ctime>

using std::string;
using std::cerr;
using std::exception;
using std::ostringstream;
using std::cout;
using std::endl;

Logger::Logger(void) :
	_filename(),
	_file(),
	_timestamp() {}

Logger::Logger(const string &filename) :
	_filename(filename),
	_file(_filename.c_str()),
	_timestamp()
{
	if (!_file.is_open())
		throw SystemErrorException("Failed to open log file");
}

Logger::Logger(const Logger &copy) :
	_filename(copy._filename),
	_file(_filename.c_str()),
	_timestamp(copy._timestamp)
{
	if (!_file.is_open())
		throw SystemErrorException("Failed to open log file");
}

Logger::~Logger(void)
{
	_file.close();
}

const string	&Logger::getFilename(void) const
{
	return _filename;
}

void	Logger::init(const string &filename)
{
	_filename = filename;
	_file.open(_filename.c_str());
	if (!_file.is_open())
		throw SystemErrorException("Failed to open log file");
}

void	Logger::logEvent(const string &message)
{
	log(message, BLUE);
}

void	Logger::logError(const exception *e)
{
	if (!e)
		return ; //per evitare potenziali loop infiniti non chiamo InternalErrorException

	const string	message(e->what());

	if (dynamic_cast<const ProtocolErrorException *>(e))
		log("User action failed: " + message, BLUE);
	else if (dynamic_cast<const InternalErrorException *>(e))
		log("Internal error: " + message, RED);
	else if (dynamic_cast<const SystemErrorException *>(e))
		log("System error: " + message, ORANGE);
	else
		log("Unknown error: " + message, YELLOW);
}

void	Logger::log(const string &message, const char *const color)
{
	updateTimestamp();
	logToConsole(message, color);
	logToFile(message);
}

void	Logger::logToFile(const string &message)
{
	if (_file.is_open())
		_file << getTimestamp() << " - " << message << endl;
}

void	Logger::logToConsole(const string &message, const char *const color) const
{
	cout << getTimestamp() << " - " << color << message << RESET << endl;
}

void	Logger::updateTimestamp(void)
{
	std::time_t	now = std::time(NULL);
	std::tm		*localTime = std::localtime(&now);
	char		buffer[80];

	std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localTime);
	_timestamp = string(buffer);
}

const string	&Logger::getTimestamp(void) const
{
	return _timestamp;
}
