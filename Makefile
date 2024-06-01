# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/26 15:33:59 by craimond          #+#    #+#              #
#    Updated: 2024/06/01 15:28:05 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER_NAME = ircserv
BOT_NAME = beetlebot

SERVER_SRCS = $(addprefix srcs/, irc.cpp	 Channel.cpp Client.cpp EventHandler.cpp Server.cpp utils.cpp SystemCalls.cpp Exceptions.cpp Logger.cpp Message.cpp)
SERVER_HDRS = $(addprefix srcs/headers/irc/, Channel.hpp Client.hpp EventHandler.hpp Server.hpp utils.hpp SystemCalls.hpp Exceptions.hpp Logger.hpp Message.hpp utils.tpp)
SERVER_OBJS = $(SRCS:.cpp=.o)

BOT_SRCS = $(addprefix srcs/, ABot.cpp Bot.cpp)
BOT_HDRS = $(addprefix srcs/headers/bot/, ABot.hpp Bot.hpp)
BOT_OBJS = $(BOT_SRCS:.o=.cpp)

INCLUDES = srcs/headers

LEAK_REPORT = leaks.log

CC = c++
VERSION = 98
CFLAGS = -g -Wall -Wextra -Werror -std=c++$(VERSION) -I$(INCLUDES)
LDFLAGS = -lssl -lcrypto
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes

RM = rm -rf

RED = \033[0;31m
GREEN = \033[0;32m
NC = \033[0m

all: $(SERVER_NAME) $(BOT_NAME)

$(SERVER_NAME): $(SERVER_OBJS) $(SERVER_HDRS)
	@$(CC) $(CFLAGS) -o $(SERVER_NAME) $(SERVER_OBJS) $(LDFLAGS)
	@echo "$(GREEN)compiled $(SERVER_NAME)$(NC)"

$(BOT_NAME): $(BOT_OBJS) $(BOT_HDRS)
	@$(CC) $(CFLAGS) -o $(BOT_NAME) $(BOT_OBJS) $(LDFLAGS)
	@echo "$(GREEN)compiled $(BOT_NAME)$(NC)"

%.o: %.cpp 
	@echo "compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(SERVER_OBJS)
	@echo "$(RED)removed $(SERVER_OBJS)$(NC)"

fclean: clean
	@$(RM) $(SERVER_NAME)
	@echo "$(RED)removed $(SERVER_NAME)$(NC)"
	@$(RM) $(LEAK_REPORT)
	@echo "$(RED)removed $(LEAK_REPORT)$(NC)"
	@$(RM) *.log
	@echo "$(RED)removed *.log$(NC)"

leaks: all
	@valgrind $(VALGRIND_FLAGS) ./$(SERVER_NAME) 9999 beet 2> $(LEAK_REPORT)
	@echo "$(GREEN)leak report generated$(NC)"

re: fclean all

.PHONY: all clean re

.SILENT: all $(SERVER_NAME) $(BOT_NAME) clean fclean re

.IGNORE: clean fclean