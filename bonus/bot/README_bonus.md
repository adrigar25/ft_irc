# ircbot

## Description

**ircbot** is a simple IRC client written in **C++98**.

It is included as an example client for the `ft_irc` project. The bot automatically connects to an IRC server, joins channels when invited, and responds to messages with predefined replies.

### Features

- Automatically replies to `PING` messages.
- Automatically joins a channel after receiving an `INVITE`.
- Replies with one of two predefined messages when:
  - Mentioned in a channel.
  - Receiving a private message.

---

## Instructions

### Requirements

- C++ compiler supporting **C++98**
- `make`

### Compilation

From the `bot/` directory:

```bash
make ircbot
```

---

### Running the bot

Basic usage:

```bash
./ircbot <nickname> <host> <port> <password>
```

Example:

```bash
./ircbot MyBot localhost 6667 mypassword
```

---

### Automatic port detection

If the server writes the listening port into a `.irc_port` file, the bot can automatically read it by using:

```bash
./ircbot MyBot localhost auto mypassword
```

---

## Limitations

- This is a simple demonstration client.
- It does not implement automatic reconnection.
- It does not implement the complete IRC protocol.
- It is intended only for testing the `ft_irc` server.

---

## Project Structure

```
bot/
├── includes/
├── src/
├── main.cpp
├── Makefile
└── README.md
```

---

## Resources

### References

- RFC 1459 – Internet Relay Chat Protocol
- RFC 2812 – Internet Relay Chat Client Protocol
- C++ Reference
- Linux Socket Programming Documentation

### AI Usage

Artificial Intelligence tools (ChatGPT) were used to:

- Explain IRC protocol behavior.
- Clarify C++98 implementation details.
- Improve the project's documentation.

The bot implementation, testing, and final behavior were developed and validated by the project authors.