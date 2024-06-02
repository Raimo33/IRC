# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: egualand <egualand@student.42firenze.it    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/26 15:33:59 by craimond          #+#    #+#              #
#    Updated: 2024/06/02 16:01:15 by egualand         #+#    #+#              #
#                                                                              #
# **************************************************************************** #

SERVER_NAME = ircserv
BOT_NAME = beetlebot

SERVER_SRCS = $(addprefix srcs/Server/, main.cpp Channel.cpp Client.cpp EventHandler.cpp Server.cpp server_utils.cpp server_exceptions.cpp)
SERVER_HDRS = $(addprefix srcs/Server/headers/, Channel.hpp Client.hpp EventHandler.hpp Server.hpp server_utils.hpp server_exceptions.hpp server_constants.hpp)
SERVER_OBJS = $(patsubst srcs/Server/%.cpp, objs/Server/%.o, $(SERVER_SRCS))
SERVER_INCS_DIR = srcs/Server/headers

BOT_SRCS = $(addprefix srcs/Bots/, main.cpp ABot.cpp BeetleBot.cpp bot_utils.cpp bot_exceptions.cpp)
BOT_HDRS = $(addprefix srcs/Bots/headers/, ABot.hpp BeetleBot.hpp bot_utils.hpp bot_exceptions.hpp bot_constants.hpp)
BOT_OBJS = $(patsubst srcs/Bots/%.cpp, objs/Bots/%.o, $(BOT_SRCS))
BOT_INCS_DIR = srcs/Bots/headers

COMMON_SRCS = $(addprefix srcs/Common/, AMessage.cpp CommandMessage.cpp ReplyMessage.cpp Logger.cpp system_calls.cpp common_utils.cpp common_exceptions.cpp)
COMMON_HDRS = $(addprefix srcs/Common/headers/, AMessage/hpp CommandMessage.hpp ReplyMessage.hpp Logger.hpp system_calls.hpp common_utils.hpp common_exceptions.hpp common_constants.hpp)
COMMON_OBJS = $(patsubst srcs/Common/%.cpp, objs/Common/%.o, $(COMMON_SRCS))
COMMON_INCS_DIR = srcs/Common/headers

LEAK_REPORT = leaks.log

CC = c++
VERSION = 98
CFLAGS = -g -Wall -Wextra -Werror -std=c++$(VERSION)
LDFLAGS = -lssl -lcrypto
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes

RM = rm -rf

RED = \033[0;31m
GREEN = \033[0;32m
NC = \033[0m

all: $(SERVER_NAME) $(BOT_NAME)

$(SERVER_NAME): $(SERVER_OBJS) $(COMMON_OBJS)
	@$(CC) $(CFLAGS) -I$(SERVER_INCS_DIR) -I$(COMMON_INCS_DIR) -o $(SERVER_NAME) $(SERVER_OBJS) $(COMMON_OBJS) $(LDFLAGS)
	@echo "$(GREEN)compiled $(SERVER_NAME)$(NC)"

$(BOT_NAME): $(BOT_OBJS) $(COMMON_OBJS)
	@$(CC) $(CFLAGS) -I$(BOT_INCS_DIR) -I$(COMMON_INCS_DIR) -o $(BOT_NAME) $(BOT_OBJS) $(COMMON_OBJS) $(LDFLAGS)
	@echo "$(GREEN)compiled $(BOT_NAME)$(NC)"

objs/Server/%.o: srcs/Server/%.cpp
	@mkdir -p $(dir $@)
	@echo "compiling $<"
	@$(CC) $(CFLAGS) -I$(SERVER_INCS_DIR) -I$(COMMON_INCS_DIR) -c $< -o $@

objs/Bots/%.o: srcs/Bots/%.cpp
	@mkdir -p $(dir $@)
	@echo "compiling $<"
	@$(CC) $(CFLAGS) -I$(BOT_INCS_DIR) -I$(COMMON_INCS_DIR) -c $< -o $@

objs/Common/%.o: srcs/Common/%.cpp
	@mkdir -p $(dir $@)
	@echo "compiling $<"
	@$(CC) $(CFLAGS) -I$(COMMON_INCS_DIR) -c $< -o $@

clean:
	@$(RM) objs
	@echo "$(RED)removed object files$(NC)"

fclean: clean
	@$(RM) $(SERVER_NAME) $(BOT_NAME)
	@echo "$(RED)removed $(SERVER_NAME) and $(BOT_NAME) binaries$(NC)"
	@$(RM) $(LEAK_REPORT)
	@echo "$(RED)removed $(LEAK_REPORT)$(NC)"
	@$(RM) *.log
	@echo "$(RED)removed log files$(NC)"

leaks: all
	@valgrind $(VALGRIND_FLAGS) ./$(SERVER_NAME) 9999 beet 2> $(LEAK_REPORT)
	@echo "$(GREEN)leak report generated$(NC)"

re: fclean all

.PHONY: all clean fclean re

.SILENT: all $(SERVER_NAME) $(BOT_NAME) clean fclean re

.IGNORE: clean fclean
