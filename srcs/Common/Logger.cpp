/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:16:50 by craimond          #+#    #+#             */
/*   Updated: 2024/06/05 18:12:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"
#include "common_exceptions.hpp"

#include <ctime>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::ostringstream;
using std::string;

Logger::Logger(void) {}

Logger::Logger(const string &filename) :
	_filename(filename),
	_file(_filename.c_str())
{
	if (!_file.is_open())
		throw SystemErrorException("Failed to open log file");
}

Logger::Logger(const Logger &copy) :
	_filename(copy._filename),
	_file(_filename.c_str())
{
	if (!_filename.empty() && !_file.is_open())
		throw SystemErrorException("Failed to open log file");
}

Logger::~Logger(void)
{
	if (!_filename.empty() && _file.is_open())
		_file.close();
}

Logger &Logger::operator=(const Logger &rhs)
{
	if (this == &rhs)
		return *this;
	_filename = rhs._filename;
	_file.open(_filename.c_str());
	if (!_file.is_open())
		throw SystemErrorException("Failed to open log file");
	return *this;
}

const string &Logger::getFilename(void) const
{
	return _filename;
}

void Logger::setFile(const string &filename)
{
	_filename = filename;
	_file.open(_filename.c_str());
	if (!_file.is_open())
		throw SystemErrorException("Failed to open log file");
}

void Logger::logEvent(const string &message) const
{
	log(message, BLUE);
}

void Logger::logError(const exception *e) const
{
	if (!e)
		return;

	const string message(e->what());

	if (dynamic_cast<const InternalErrorException *>(e))
		log("Internal error: " + message, RED);
	else if (dynamic_cast<const SystemErrorException *>(e))
		log("System error: " + message, ORANGE);
	else
		log("Unknown error: " + message, YELLOW);
}

void Logger::log(const string &message, const char *const color) const
{
	const string &timestamp = getTimestamp();
	logToConsole(timestamp, message, color);
	logToFile(timestamp, message);
}

void Logger::logToFile(const string &timestamp, const string &message) const
{
	if (!_filename.empty() && _file.is_open())
		_file << timestamp << " - " << message << endl;
}

void Logger::logToConsole(const string &timestamp, const string &message, const char *const color) const
{
	cout << timestamp << " - " << color << message << RESET << endl;
}

const string Logger::getTimestamp(void) const
{
	std::time_t now = std::time(NULL);
	std::tm	   *localTime = std::localtime(&now);
	char		buffer[80];

	std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localTime);
	return string(buffer);
}
