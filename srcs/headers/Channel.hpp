/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 19:07:03 by craimond          #+#    #+#             */
/*   Updated: 2024/05/02 00:04:46 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# define N_MODES 5
typedef enum channel_modes
{
	MODE_I = 0,
	MODE_T = 1,
	MODE_K = 2,
	MODE_O = 3,
	MODE_L = 4
}	e_channel_modes;

# include <string>
# include <vector>

using namespace std;

class ChannelOperator;

class Channel
{
	public:
		Channel();
		Channel(const Channel &copy);
		~Channel();
		Channel	&operator=(const Channel &rhs);
		bool	getMode(const channel_modes &mode) const;
		string	getTopic() const;
	private:
		//solo l'operator puo' cambiare modes e topic del canale (operator sara' un friend di Channel)
		friend class ChannelOperator;
		void	setMode(const ChannelOperator &op, const channel_modes &mode, const bool status);
		void	setTopic(const string &new_topic);
		vector<ChannelOperator *>	_operators; //serve per sapere se l'operator che sta provando a cambiare roba e' effettivamente un operator di quel canale
		bool						_modes[N_MODES];
		string						_topic;
};

#endif