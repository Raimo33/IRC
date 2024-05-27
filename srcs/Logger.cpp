/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:16:50 by craimond          #+#    #+#             */
/*   Updated: 2024/05/27 13:47:53 by craimond         ###   ########.fr       */
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

using namespace irc;

namespace irc
{
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
		if (!_file.is_open())
			throw SystemErrorException("Failed to open log file");
	}

	Logger::~Logger(void)
	{
		_file.close();
	}

	void	Logger::logEvent(const string &message)
	{
		_file << message << endl;
		cout << message << endl;
	}

	void	Logger::logError(const exception *e)
	{
		if (!e)
			return ; //per evitare potenziali loop infiniti non chiamo InternalErrorException

		ostringstream	to_log;

		if (dynamic_cast<const ProtocolErrorException *>(e))
			to_log << "Protocol error: " << e->what();
		else if (dynamic_cast<const InternalErrorException *>(e))
			to_log << "Internal error: " << e->what();
		else if (dynamic_cast<const SystemErrorException *>(e))
			to_log << "System error: " << e->what();
		else
			to_log << "Unknown error: " << e->what();
		log(to_log.str());
	}

	void	Logger::log(const string &message)
	{
		const string timestamp = getTimestamp();

		if (_file.is_open())
			_file << timestamp << " - " << message << endl;
		cerr << timestamp << " - " << message << endl;
	}

	const string Logger::getTimestamp(void) const
	{
		std::time_t		now = std::time(NULL);
		std::tm			*localTime = std::localtime(&now);
		char			buffer[80];

		std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localTime);
		return string(buffer);
	}
}
