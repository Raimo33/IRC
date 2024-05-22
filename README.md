## Project Description

An Internet Relay Chat (IRC) server compatible with RFCs 1459, 2810, 2811, 2812, and 2813.

This project implements a fully-featured IRC server that adheres to the specifications outlined in the following RFCs:
- **[RFC 1459](https://tools.ietf.org/html/rfc1459)**: The original IRC protocol.
- **[RFC 2810](https://tools.ietf.org/html/rfc2810)**: Describes the architecture of IRC.
- **[RFC 2811](https://tools.ietf.org/html/rfc2811)**: Defines the channel management aspects of IRC.
- **[RFC 2812](https://tools.ietf.org/html/rfc2812)**: Details the client protocol used in IRC.
- **[RFC 2813](https://tools.ietf.org/html/rfc2813)**: Provides specifications for the IRC server protocol.

### Features

- Full support for user authentication and registration.
- Channel management, including creation, moderation, and user roles.
- Secure communication through SSL/TLS.
- Extensible commands and modes for both users and channels.
- Logging and monitoring capabilities for server administration.
- Compatibility with popular IRC clients.

### Getting Started

1. **Clone the repository:**
    ```sh
    git clone https://github.com/Raimo33/IRC.git
    ```

2. **Build the server:**
    ```sh
    cd IRC
    make
    ```

3. **Run the server:**
    ```sh
    ./ircserv
    ```

### Documentation

Detailed documentation is available in the [Wiki](https://github.com/Raimo33/IRC/wiki). It includes installation guides, configuration options, and usage instructions.

### Contributing

Contributions are welcome! Please read our [Contributing Guidelines](https://github.com/Raimo33/IRC/blob/main/CONTRIBUTING.md) before submitting a pull request.

### License

This project is licensed under the MIT License. See the [LICENSE](https://github.com/Raimo33/IRC/blob/main/LICENSE) file for details.
