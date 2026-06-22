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
	@echo "\033[35m$(NAME) executable is ready to use!\033[0m"

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

# Rules
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@$(CC) -Wall -Wextra -Werror -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@printf 'Eliminando objetos:\033[0;32m %s\033[0m\n' "$(OBJS)"
	@rm -f $(OBJS)

fclean: clean
	@printf 'Eliminando ejecutable:\033[0;32m %s\033[0m\n' "$(NAME)"
	@rm -f $(NAME) ircbot
	@rm -rf $(OBJDIR)

re: fclean all
	@if [ $$? -eq 0 ]; then \
		echo "\033[38;5;208m🎉 Recompilation completed successfully!\033[0m"; \
	else \
		echo "\033[31m❌ Error during recompilation!\033[0m"; \
	fi

.PHONY: all clean fclean re ircbot

