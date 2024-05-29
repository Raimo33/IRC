## Project Description

An basic Internet Relay Chat (IRC) server compatible with RFCs [1459](https://tools.ietf.org/html/rfc1459), [2810](https://tools.ietf.org/html/rfc2810), [2811](https://tools.ietf.org/html/rfc2811), [2812](https://tools.ietf.org/html/rfc), and [2813](https://tools.ietf.org/html/rfc2813).

### Features

- Full support for user authentication and registration.
- Channel management, including creation, moderation.
- Extensible commands and modes for both users and channels.
- Logging capabilities.
- File transfer.
- Chatbot.
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
    ./ircserv <port> <password>
    ```

### Documentation

Detailed documentation is available in the [Wiki](https://github.com/Raimo33/IRC/wiki). It includes installation guides, configuration options, and usage instructions.

### Contributing

Contributions are welcome! Please read our [Contributing Guidelines](https://github.com/Raimo33/IRC/blob/main/CONTRIBUTING.md) before submitting a pull request.

### License

This project is licensed under the MIT License. See the [LICENSE](https://github.com/Raimo33/IRC/blob/main/LICENSE) file for details.
