## Project Description

A basic Internet Relay Chat (IRC) server and Automated Client (Bot) compatible with RFCs [1459](https://tools.ietf.org/html/rfc1459), [2810](https://tools.ietf.org/html/rfc2810), [2811](https://tools.ietf.org/html/rfc2811), [2812](https://tools.ietf.org/html/rfc), and [2813](https://tools.ietf.org/html/rfc2813).

### Features

- Full support for user authentication and registration.
- Channel management, including creation, moderation.
- Extensible commands and modes for both users and channels.
- Logging capabilities.
- File transfer.
- Bot building support.
- Compatibility with popular IRC clients.

### Getting Started

1. **Clone the repository:**
    ```sh
    git clone https://github.com/Raimo33/IRC.git
    CD ft_irc
    ```

2. **Build the server:**
    ```sh
    make ircserv
    ```

3. **Run the server:**
    ```sh
    ./ircserv <port> <password>
    ```
4. **Build the bot:**
    ```sh
    make beetlebot
    ```
3. **Run the bot:**
    ```sh
    ./beetlebot <ip> <port> [<password>]
    ```

### Documentation

Detailed documentation is available in the [Wiki](https://github.com/Raimo33/IRC/wiki). It includes installation guides, configuration options, and usage instructions.

### IRC Message Format Cheat Sheet

```plaintext
<message> ::= [':' <prefix> <SPACE>] <command> <params> <crlf>
<prefix> ::= <servername> | <nick> ['!' <user>] ['@' <host>]
<command> ::= <letter> {<letter>} | <number> <number> <number>
<params> ::= <SPACE> [':' <trailing> | <middle> <params>]

<middle> ::= <Any *non-empty* sequence of octets not including SPACE
           or NUL or CR or LF, the first of which may not be ':'>
<trailing> ::= <Any, possibly *empty*, sequence of octets not including
             NUL or CR or LF>

<SPACE> ::= ' ' { ' ' }
<crlf> ::= CR LF
