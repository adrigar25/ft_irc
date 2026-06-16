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

mandatory: $(NAME)

bonus: $(NAME)_bonus force_bot

force_bot:
	$(MAKE) -C $(BOT_DIR)

# ---------------- EXECUTABLES ----------------
$(NAME): $(MANDATORY_OBJS)
	$(CPP) $(CPPFLAGS) $(INCLUDES) $^ -o $@

$(NAME)_bonus: $(BONUS_OBJS)
	$(CPP) $(CPPFLAGS) $(INCLUDES_BONUS) $^ -o $@

# ---------------- BOT ----------------
$(BOT_DIR)/bot:
	$(MAKE) -C $(BOT_DIR)

# ---------------- COMPILE RULES ----------------
$(OBJS_DIR)/mandatory/%.o: mandatory/%.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR)/bonus/%.o: bonus/%.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(INCLUDES_BONUS) -c $< -o $@

# ---------------- CLEAN ----------------
clean:
	@rm -rf $(OBJS_DIR)
	@$(MAKE) -C $(BOT_DIR) clean

fclean: clean
	@rm -f $(NAME) $(NAME)
	@$(MAKE) -C $(BOT_DIR) fclean

re: fclean all

.PHONY: all mandatory bonus clean fclean re
