# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/17 18:24:30 by adriescr          #+#    #+#              #
#    Updated: 2026/06/22 20:07:38 by agarcia          ###   ########.fr        #
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

# ---------------- RULES ----------------

all:
	@echo "\033[33mCompiling mandatory...\033[0m"
	@$(MAKE) --no-print-directory $(NAME)

mandatory: all

bonus:
	@echo "\033[33mCompiling bonus...\033[0m"
	@$(MAKE) --no-print-directory $(NAME)_bonus
	@$(MAKE) --no-print-directory -C $(BOT_DIR)
	@echo "\033[32m✓ $(NAME)_bonus compiled successfully\033[0m"

force_bot:
	@$(MAKE) -C $(BOT_DIR)

# ---------------- EXECUTABLES ----------------

$(NAME): $(MANDATORY_OBJS)
	@$(CPP) $(CPPFLAGS) $(INCLUDES) $^ -o $@
	@echo "\033[32m✓ $(NAME) compiled successfully\033[0m"

$(NAME)_bonus: $(BONUS_OBJS)
	@$(CPP) $(CPPFLAGS) $(INCLUDES_BONUS) $^ -o $@
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
	@rm -rf $(OBJS_DIR)
	@$(MAKE) -C $(BOT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@rm -f $(NAME)_bonus
	@$(MAKE) -C $(BOT_DIR) fclean

re: fclean all

# ---------------- PHONY ----------------

.PHONY: all mandatory bonus clean fclean re force_bot
