/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hasher.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:57:54 by craimond          #+#    #+#             */
/*   Updated: 2024/05/21 19:41:37 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc/irc.hpp"
#include "irc/Hasher.hpp"

using namespace std;
using namespace irc;

Hasher::Hasher(void)
{
	init();
}

Hasher::Hasher(const string &text)
{
	init();
	update(text.c_str(), text.length());
	finalize();
}

void Hasher::init(void)
{
	finalized = false;
	count[0] = 0;
	count[1] = 0;
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
}

void Hasher::update(const unsigned char *input, size_type length)
{
    size_type index = count[0] / 8 % blocksize;
    if ((count[0] += (length << 3)) < (length << 3))
        count[1]++;
    count[1] += (length >> 29);

    size_type firstpart = 64 - index;
    size_type i;

    if (length >= firstpart) {
        memcpy(&buffer[index], input, firstpart);
        transform(buffer);
        for (i = firstpart; i + blocksize <= length; i += blocksize)
            transform(&input[i]);
        index = 0;
    } else
        i = 0;

    memcpy(&buffer[index], &input[i], length - i);
}

void Hasher::update(const char *input, size_type length)
{
	update((const unsigned char *)input, length);
}

Hasher& Hasher::finalize(void)
{
    static unsigned char bits[8];
    encode(bits, count, 8);

    size_type index = count[0] / 8 % 64;
    size_type padLen = (index < 56) ? (56 - index) : (120 - index);
    update(PADDING, padLen);
    update(bits, 8);

    encode(digest, state, 16);
    memset(buffer, 0, sizeof buffer);
    memset(count, 0, sizeof count);

    finalized = true;
    return *this;
}

string Hasher::hexdigest(void) const
{
    if (!finalized)
        return "";

    char buf[33];
    for (int i = 0; i < 16; i++) {
        buf[i * 2] = HEX_NUMBERS[(digest[i] >> 4) & 0x0f];
        buf[i * 2 + 1] = HEX_NUMBERS[digest[i] & 0x0f];
    }
    buf[32] = 0;

    return string(buf);
}

void Hasher::transform(const uint1 block[blocksize])
{
    uint4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    decode(x, block, blocksize);

    FF(a, b, c, d, x[0], 7, 0xd76aa478);
    FF(d, a, b, c, x[1], 12, 0xe8c7b756);
    FF(c, d, a, b, x[2], 17, 0x242070db);
    FF(b, c, d, a, x[3], 22, 0xc1bdceee);
    FF(a, b, c, d, x[4], 7, 0xf57c0faf);
    FF(d, a, b, c, x[5], 12, 0x4787c62a);
    FF(c, d, a, b, x[6], 17, 0xa8304613);
    FF(b, c, d, a, x[7], 22, 0xfd469501);
    FF(a, b, c, d, x[8], 7, 0x698098d8);
    FF(d, a, b, c, x[9], 12, 0x8b44f7af);
    FF(c, d, a, b, x[10], 17, 0xffff5bb1);
    FF(b, c, d, a, x[11], 22, 0x895cd7be);
    FF(a, b, c, d, x[12], 7, 0x6b901122);
    FF(d, a, b, c, x[13], 12, 0xfd987193);
    FF(c, d, a, b, x[14], 17, 0xa679438e);
    FF(b, c, d, a, x[15], 22, 0x49b40821);

    GG(a, b, c, d, x[1], 5, 0xf61e2562);
    GG(d, a, b, c, x[6], 9, 0xc040b340);
    GG(c, d, a, b, x[11], 14, 0x265e5a51);
    GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);
    GG(a, b, c, d, x[5], 5, 0xd62f105d);
    GG(d, a, b, c, x[10], 9, 0x02441453);
    GG(c, d, a, b, x[15], 14, 0xd8a1e681);
    GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);
    GG(a, b, c, d, x[9], 5, 0x21e1cde6);
    GG(d, a, b, c, x[14], 9, 0xc33707d6);
    GG(c, d, a, b, x[3], 14, 0xf4d50d87);
    GG(b, c, d, a, x[8], 20, 0x455a14ed);
    GG(a, b, c, d, x[13], 5, 0xa9e3e905);
    GG(d, a, b, c, x[2], 9, 0xfcefa3f8);
    GG(c, d, a, b, x[7], 14, 0x676f02d9);
    GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);

    HH(a, b, c, d, x[5], 4, 0xfffa3942);
    HH(d, a, b, c, x[8], 11, 0x8771f681);
    HH(c, d, a, b, x[11], 16, 0x6d9d6122);
    HH(b, c, d, a, x[14], 23, 0xfde5380c);
    HH(a, b, c, d, x[1], 4, 0xa4beea44);
    HH(d, a, b, c, x[4], 11, 0x4bdecfa9);
    HH(c, d, a, b, x[7], 16, 0xf6bb4b60);
    HH(b, c, d, a, x[10], 23, 0xbebfbc70);
    HH(a, b, c, d, x[13], 4, 0x289b7ec6);
    HH(d, a, b, c, x[0], 11, 0xeaa127fa);
    HH(c, d, a, b, x[3], 16, 0xd4ef3085);
    HH(b, c, d, a, x[6], 23, 0x04881d05);
    HH(a, b, c, d, x[9], 4, 0xd9d4d039);
    HH(d, a, b, c, x[12], 11, 0xe6db99e5);
    HH(c, d, a, b, x[15], 16, 0x1fa27cf8);
    HH(b, c, d, a, x[2], 23, 0xc4ac5665);

    II(a, b, c, d, x[0], 6, 0xf4292244);
    II(d, a, b, c, x[7], 10, 0x432aff97);
    II(c, d, a, b, x[14], 15, 0xab9423a7);
    II(b, c, d, a, x[5], 21, 0xfc93a039);
    II(a, b, c, d, x[12], 6, 0x655b59c3);
    II(d, a, b, c, x[3], 10, 0x8f0ccc92);
    II(c, d, a, b, x[10], 15, 0xffeff47d);
    II(b, c, d, a, x[1], 21, 0x85845dd1);
    II(a, b, c, d, x[8], 6, 0x6fa87e4f);
    II(d, a, b, c, x[15], 10, 0xfe2ce6e0);
    II(c, d, a, b, x[6], 15, 0xa3014314);
    II(b, c, d, a, x[13], 21, 0x4e0811a1);
    II(a, b, c, d, x[4], 6, 0xf7537e82);
    II(d, a, b, c, x[11], 10, 0xbd3af235);
    II(c, d, a, b, x[2], 15, 0x2ad7d2bb);
    II(b, c, d, a, x[9], 21, 0xeb86d391);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    memset(x, 0, sizeof x);
}

void Hasher::encode(uint1 *output, const uint4 *input, size_type len)
{
    for (size_type i = 0, j = 0; j < len; ++i, j += 4)
	{
        output[j] = input[i] & 0xff;
        output[j + 1] = (input[i] >> 8) & 0xff;
        output[j + 2] = (input[i] >> 16) & 0xff;
        output[j + 3] = (input[i] >> 24) & 0xff;
    }
}

void Hasher::decode(uint4 *output, const uint1 *input, size_type len)
{
    for (size_type i = 0, j = 0; j < len; ++i, j += 4)
	{
        output[i] = ((uint4)input[j]) | (((uint4)input[j + 1]) << 8) |
                    (((uint4)input[j + 2]) << 16) | (((uint4)input[j + 3]) << 24);
    }
}

inline Hasher::uint4 Hasher::F(uint4 x, uint4 y, uint4 z)
{
    return (x & y) | (~x & z);
}

inline Hasher::uint4 Hasher::G(uint4 x, uint4 y, uint4 z)
{
    return (x & z) | (y & ~z);
}

inline Hasher::uint4 Hasher::H(uint4 x, uint4 y, uint4 z)
{
    return x ^ y ^ z;
}

inline Hasher::uint4 Hasher::I(uint4 x, uint4 y, uint4 z)
{
    return y ^ (x | ~z);
}

inline Hasher::uint4 Hasher::rotate_left(uint4 x, int n)
{
    return (x << n) | (x >> (32 - n));
}

inline void Hasher::FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{
    a = rotate_left(a + F(b, c, d) + x + ac, s) + b;
}

inline void Hasher::GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{
    a = rotate_left(a + G(b, c, d) + x + ac, s) + b;
}

inline void Hasher::HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{
    a = rotate_left(a + H(b, c, d) + x + ac, s) + b;
}

inline void Hasher::II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac)
{
    a = rotate_left(a + I(b, c, d) + x + ac, s) + b;
}


