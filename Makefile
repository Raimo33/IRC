# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/26 15:33:59 by craimond          #+#    #+#              #
#    Updated: 2024/06/01 18:31:43 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SERVER_NAME = ircserv
BOT_NAME = beetlebot

SERVER_SRCS = $(addprefix srcs/Server/, main.cpp Channel.cpp Client.cpp EventHandler.cpp Server.cpp Exceptions.cpp)
SERVER_HDRS = $(addprefix srcs/Server/headers/,  Channel.hpp Client.hpp EventHandler.hpp Server.hpp Exceptions.hpp)
SERVER_OBJS = $(SRCS:.cpp=.o)
SERVER_INCS_DIR = srcs/Server/headers

BOT_SRCS = $(addprefix srcs/, main.cpp ABot.cpp BeetleJuice.cpp)
BOT_HDRS = $(addprefix srcs/headers/bot/, ABot.hpp BeetleJuice.hpp)
BOT_OBJS = $(BOT_SRCS:.o=.cpp)
BOT_INCS_DIR = srcs/Bots/headers

COMMON_SRCS = $(addprefix srcs/Common/, Logger.cpp Message.cpp SystemCalls.cpp Utils.cpp)
COMMON_HDRS = $(addprefix srcs/Common/headers/, Logger.hpp Message.hpp SystemCalls.hpp Utils.hpp)
COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
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

$(SERVER_NAME): $(SERVER_OBJS) $(COMMON_OBJS) $(SERVER_HDRS) $(COMMON_HDRS)
	@$(CC) $(CFLAGS) -I$(SERVER_INCS_DIR) -I$(COMMON_INCS_DIR) -o $(SERVER_NAME) $(SERVER_OBJS) $(COMMON_OBJS) $(LDFLAGS)
	@echo "$(GREEN)compiled $(SERVER_NAME)$(NC)"

$(BOT_NAME): $(BOT_OBJS) $(COMMON_OBJS) $(BOT_HDRS) $(COMMON_HDRS)
	@$(CC) $(CFLAGS) -I$(BOT_INCS_DIR) -I$(COMMON_INCS_DIR) -o $(BOT_NAME) $(BOT_OBJS) $(COMMON_OBJS) $(LDFLAGS)
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