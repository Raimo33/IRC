/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:15:37 by craimond          #+#    #+#             */
/*   Updated: 2024/05/27 13:47:57 by craimond         ###   ########.fr       */
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

namespace irc
{
	class Logger
	{
		public:
			explicit Logger(const std::string &filename);
			Logger(const Logger &copy);
			~Logger(void);

			void				logEvent(const std::string &message);
			void				logError(const std::exception *e);

		private:
			void				log(const std::string &message);
			const std::string	getTimestamp(void) const;

			const std::string	_filename;
			std::ofstream		_file;
	};
}

#endif