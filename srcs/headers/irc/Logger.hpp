/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/06/01 16:05:28 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <exception>
# include <sstream>
# include <iomanip>

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

		Logger				&operator=(const Logger &rhs);

		const std::string	&getFilename(void) const;		
		void				setFile(const std::string &filename);

		void				logEvent(const std::string &message);
		void				logError(const std::exception *e);

	private:
		void				log(const std::string &message, const char *const color = "");
		void				logToFile(const std::string &message);
		void				logToConsole(const std::string &message, const char *const color = "") const;
		void				updateTimestamp(void);
		const std::string	&getTimestamp(void) const;

		std::string			_filename;
		std::ofstream		_file;
		std::string			_timestamp;
};

#endif