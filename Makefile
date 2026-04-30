# Simple Makefile for ft_irc
NAME = ft_irc

CXX = g++
CC = gcc

CXXFLAGS = -Wall -Wextra -std=c++98 -I includes
LDFLAGS =

SRCDIR = src
INCDIR = includes

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SRCS:.cpp=.o)

CSRCS := $(wildcard $(SRCDIR)/*.c)
COBJS := $(CSRCS:.c=.o)

HEADERS := $(wildcard $(INCDIR)/*.hpp)

all: $(NAME)

$(NAME): $(OBJS) $(COBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(COBJS) -o $(NAME) $(LDFLAGS)

# Rules
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -Wall -Wextra -Werror -c $< -o $@

clean:
	rm -f $(OBJS) $(COBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

