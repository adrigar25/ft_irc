# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/17 18:24:30 by adriescr          #+#    #+#              #
#    Updated: 2026/06/17 18:33:23 by adriescr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ircserv

CPP := c++
CPPFLAGS := -Wall -Wextra -Werror -std=c++98

INCLUDES := -I mandatory/includes
INCLUDES_BONUS := -I bonus/includes

OBJS_DIR := objs

MANDATORY_DIR := mandatory
BONUS_DIR := bonus
BOT_DIR := bonus/bot

# ---------------- SOURCES ----------------
MANDATORY_SRC := $(shell find $(MANDATORY_DIR)/src -type f -name "*.cpp")
MANDATORY_MAIN := $(MANDATORY_DIR)/main.cpp
MANDATORY_ALL := $(MANDATORY_SRC) $(MANDATORY_MAIN)

BONUS_SRC := $(shell find $(BONUS_DIR)/src -type f -name "*.cpp")
BONUS_MAIN := $(BONUS_DIR)/main.cpp
BONUS_ALL := $(BONUS_SRC) $(BONUS_MAIN)

# ---------------- OBJECTS ----------------
MANDATORY_OBJS := $(patsubst $(MANDATORY_DIR)/%.cpp,$(OBJS_DIR)/mandatory/%.o,$(MANDATORY_ALL))
BONUS_OBJS := $(patsubst $(BONUS_DIR)/%.cpp,$(OBJS_DIR)/bonus/%.o,$(BONUS_ALL))

# ---------------- DEFAULT ----------------
all: mandatory
	@echo "\033[35m$(NAME) executable is ready to use!\033[0m"

mandatory: $(NAME)
	@echo "\033[35m$(NAME) executable is ready to use!\033[0m"

bonus: $(NAME)_bonus force_bot
	@echo "\033[35m$(NAME)_bonus executable is ready to use!\033[0m"

force_bot:
	@$(MAKE) -C $(BOT_DIR)

# ---------------- EXECUTABLES ----------------
$(NAME): $(MANDATORY_OBJS)
	@$(CPP) $(CPPFLAGS) $(INCLUDES) $^ -o $@
	@if [ $$? -eq 0 ]; then \
		echo "Error: Failed to compile $<"; \
		exit 1; \
	fi

$(NAME)_bonus: $(BONUS_OBJS)
	@$(CPP) $(CPPFLAGS) $(INCLUDES_BONUS) $^ -o $@
	@if [ $$? -eq 0 ]; then \
		echo "Error: Failed to compile $<"; \
		exit 1; \
	fi

# ---------------- BOT ----------------
$(BOT_DIR)/bot:
	@$(MAKE) -C $(BOT_DIR)

# ---------------- COMPILE RULES ----------------
$(OBJS_DIR)/mandatory/%.o: mandatory/%.cpp
	@mkdir -p $(dir $@)
	@$(CPP) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR)/bonus/%.o: bonus/%.cpp
	@mkdir -p $(dir $@)
	@$(CPP) $(CPPFLAGS) $(INCLUDES_BONUS) -c $< -o $@

# ---------------- CLEAN ----------------
clean:
	@printf 'Limpiando objetos:\033[0;32m %s\033[0m\n' "$(NAME)"
	@rm -rf $(OBJS_DIR)
	@$(MAKE) -C $(BOT_DIR) clean

fclean: clean
	@printf 'Eliminando ejecutable:\033[0;32m %s\033[0m\n' "$(NAME)"
	@rm -f $(NAME)
	@$(MAKE) -C $(BOT_DIR) fclean

re: fclean all
	@if [ $$? -eq 0 ]; then \
		echo "\033[38;5;208m🎉 Recompilation completed successfully!\033[0m"; \
	else \
		echo "\033[31m❌ Error during recompilation!\033[0m"; \
	fi

.PHONY: all mandatory bonus clean fclean re
