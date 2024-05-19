# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/26 15:33:59 by craimond          #+#    #+#              #
#    Updated: 2024/05/19 17:28:46 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS = $(addprefix srcs/, irc.cpp Channel.cpp ChannelOperator.cpp Client.cpp EventHandler.cpp IMessage.cpp Message.cpp PrivateMessage.cpp Server.cpp User.cpp utils.cpp SystemCalls.cpp Hasher.cpp)
OBJS = $(SRCS:.cpp=.o)
HDRS = $(addprefix srcs/headers/, Channel.hpp ChannelOperator.hpp Client.hpp EventHandler.hpp IMessage.hpp Message.hpp PrivateMessage.hpp Server.hpp User.hpp utils.hpp SystemCalls.hpp Hasher.hpp)
LEAK_REPORT = leaks.log

CC = c++
VERSION = 98
CFLAGS = -Wall -Wextra -Werror -std=c++$(VERSION)
VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes

RM = rm -rf

RED = \033[0;31m
GREEN = \033[0;32m
NC = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)compiled $(NAME)$(NC)"

%.o: %.cpp $(HDRS)
	@echo "compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)
	@echo "$(RED)removed $(OBJS)$(NC)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)removed $(NAME)$(NC)"
	@$(RM) $(LEAK_REPORT)
	@echo "$(RED)removed $(LEAK_REPORT)$(NC)"

leaks: all
	@valgrind $(VALGRIND_FLAGS) ./$(NAME) 2> $(LEAK_REPORT)
	@echo "$(GREEN)leak report generated$(NC)"

re: fclean all

.PHONY: all clean re

.SILENT: all $(NAME) $(OBJS) clean fclean re

.IGNORE: clean fclean