# Simple Makefile for ft_irc
NAME = ft_irc

CXX = g++
CC = gcc

CXXFLAGS = -Wall -Wextra -std=c++98 -I includes
LDFLAGS =

SRCDIR = src
INCDIR = includes

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJDIR = objs
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

CSRCS := $(wildcard $(SRCDIR)/*.c)
COBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(CSRCS))

HEADERS := $(wildcard $(INCDIR)/*.hpp)

all: $(NAME)

$(NAME): $(OBJS) $(COBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(COBJS) -o $(NAME) $(LDFLAGS)

# Rules
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -Wall -Wextra -Werror -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJS) $(COBJS)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR)

re: fclean all

.PHONY: all clean fclean re

