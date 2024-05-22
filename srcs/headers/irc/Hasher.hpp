/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hasher.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:55:59 by craimond          #+#    #+#             */
/*   Updated: 2024/05/22 21:54:47 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHER_HPP
# define HASHER_HPP

#include <cstring>
#include <string>
#include <stdint.h>

namespace irc
{
	//TODO MD% e' considerato broken e insicuro, sostituire con SHA-256
	class Hasher
	{
		public:
			static std::string		hash(const std::string &text);

		private:
			typedef unsigned int size_type;

			explicit Hasher(const std::string &text);

			void					update(const unsigned char *buf, size_type length);
			void					update(const char *buf, size_type length);
			Hasher					&finalize(void);
			std::string				hexdigest(void) const;
			void					init(void);

			enum { blocksize = 64 };

			void					transform(const uint8_t block[blocksize]);
			static void				decode(uint32_t output[], const uint8_t input[], size_type len);
			static void				encode(uint8_t output[], const uint32_t input[], size_type len);

			bool					finalized;
			uint8_t					buffer[blocksize];
			uint32_t				count[2];
			uint32_t				state[4];
			uint8_t					digest[16];

			static inline uint32_t	F(uint32_t x, uint32_t y, uint32_t z);
			static inline uint32_t	G(uint32_t x, uint32_t y, uint32_t z);
			static inline uint32_t	H(uint32_t x, uint32_t y, uint32_t z);
			static inline uint32_t	I(uint32_t x, uint32_t y, uint32_t z);
			static inline uint32_t	rotate_left(uint32_t x, int n);
			static inline void		FF(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
			static inline void		GG(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
			static inline void		HH(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
			static inline void		II(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
	};
}

const unsigned char PADDING[64] = { 0x80 };
const char HEX_NUMBERS[] = "0123456789abcdef";

#endif
