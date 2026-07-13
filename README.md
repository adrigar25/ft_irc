*This project has been created as part of the 42 curriculum by agarcia, adriescr, simgarci
# ft_irc

## Description

**ft_irc** is an Internet Relay Chat (IRC) server written in **C++98**.

The goal of this project is to understand how network servers work by implementing the core features of the IRC protocol using a single `poll()` instance for non-blocking I/O. The server supports client authentication, channel management, private messaging, channel modes, invitations, topic management, and operator commands.

### Features

- IRC client authentication (`PASS`, `NICK`, `USER`)
- Channel creation and management
- Private and channel messaging
- Channel operators
- Channel modes (`i`, `t`, `k`, `o`, `l`, `b`)
- User invitations
- Kick users
- Topic management
- List available channels
- Non-blocking sockets using `poll()`
- Compatible with IRC clients such as HexChat, irssi and WeeChat

---

## Instructions

### Requirements

- C++ compiler supporting **C++98**
- `make`

### Compilation

```bash
make
```

Useful targets:

```bash
make
make clean
make fclean
make re
```

The executable generated is:

```bash
ircserv
```

---

### Running the server

The server can be started in two different ways.

Using a custom port:

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 mypassword
```

Or using the default port (`6667`):

```bash
./ircserv <password>
```

Example:

```bash
./ircserv mypassword
```

---

### Connecting to the server

You can connect using any IRC client (HexChat, irssi, WeeChat...) or directly with `netcat`.

Example:

```bash
nc localhost 6667

PASS mypassword
NICK alice
USER alice 0 * :Alice
```

---

### Authentication

A client becomes fully registered after sending:

```text
PASS <password>
NICK <nickname>
USER <username> <mode> <unused> :<realname>
```

Once authenticated, the server sends the IRC welcome message (`001`).

---

## Supported Commands

### Authentication

```
PASS
NICK
USER
```

### Channel Management

```
JOIN
PART
LIST
INVITE
KICK
MODE
TOPIC
```

### Messaging

```
PRIVMSG
```

### Session Management

```
PING
QUIT
```

Unknown commands generate the IRC numeric reply:

```
421 Unknown command
```

---

## Channel Modes

The following channel modes are implemented:

| Mode | Description |
|------|-------------|
| `+i` | Invite-only channel |
| `+t` | Only operators may change the topic |
| `+k` | Password-protected channel |
| `+o` | Grant or remove channel operator |
| `+l` | User limit |
| `+b` | Ban / unban users |

---

## Important IRC Replies

Some important numeric replies implemented are:

| Code | Description |
|------|-------------|
| 001 | Welcome |
| 331 | No topic is set |
| 332 | Channel topic |
| 353 | Names list |
| 366 | End of names list |
| 401 | No such nick |
| 403 | No such channel |
| 404 | Cannot send to channel |
| 421 | Unknown command |
| 431 | No nickname given |
| 433 | Nickname already in use |
| 441 | User isn't on that channel |
| 461 | Not enough parameters |
| 471 | Channel is full |
| 473 | Invite only |
| 474 | Banned from channel |
| 475 | Bad channel key |
| 482 | You're not channel operator |

---

## Limitations

- Maximum of **10 channels per user**.
- No persistence between executions.
- No SSL/TLS support.
- Uses a single `poll()` instance.
- The `CAP` command exists but is currently not registered in the command dispatcher.

---

## Example Session

Server:

```bash
make
./ircserv 6667 mypassword
```

Client:

```bash
nc localhost 6667

PASS mypassword
NICK alice
USER alice 0 * :Alice
JOIN #general
PRIVMSG #general :Hello everyone!
```

---

## Project Structure

```
.
├── includes/
├── src/
├── bot/
├── main.cpp
├── Makefile
└── README.md
```

---

## Resources

### IRC Documentation

- RFC 1459 – Internet Relay Chat Protocol
- RFC 2810 – IRC Architecture
- RFC 2811 – Channel Management
- RFC 2812 – Client Protocol
- Beej's Guide to Network Programming
- Linux `poll(2)` manual pages
- C++ reference documentation

### AI Usage

Artificial Intelligence tools (ChatGPT) were used during the development of this project to:

- Clarify the IRC protocol specification.
- Better understand socket programming concepts.
- Explain the behavior of `poll()` and non-blocking sockets.
- Resolve C++98 syntax questions.
- Review code structure and suggest refactoring ideas.
- Improve the documentation and write this README.

All implementation decisions, architecture, debugging, testing, and final code were developed and validated by the project authors.