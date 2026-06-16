# Simple Makefile for ft_irc
NAME = bot_ircserv

CXX = g++
CC = gcc

CXXFLAGS = -Wall -Wextra -std=c++98 -I includes
LDFLAGS =

SRCDIR = src
INCDIR = includes

SRCS := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp) $(wildcard $(SRCDIR)/*/*/*.cpp)
OBJDIR = objs
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

HEADERS := $(wildcard $(INCDIR)/*.hpp)

all: ircbot

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

# Rules
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) -Wall -Wextra -Werror -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJS) ircbot

fclean: clean
	rm -f $(NAME) ircbot
	rm -rf $(OBJDIR)

ircbot:
	$(CXX) -std=c++11 -Wall -Wextra -I $(INCDIR) main.cpp $(SRCDIR)/Bot.cpp $(SRCDIR)/IRCConnection.cpp $(SRCDIR)/ChannelManager.cpp $(SRCDIR)/RequestHandler.cpp -o ircbot

re: fclean all

.PHONY: all clean fclean re ircbot

