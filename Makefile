# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/26 15:33:59 by craimond          #+#    #+#              #
#    Updated: 2024/05/27 13:36:48 by craimond         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS = $(addprefix srcs/, 				Channel.cpp ChannelOperator.cpp Client.cpp EventHandler.cpp Server.cpp utils.cpp SystemCalls.cpp Hasher.cpp Exceptions.cpp irc.cpp Logger.cpp ReplyCodes.cpp)
HDRS = $(addprefix srcs/headers/irc/,	Channel.hpp ChannelOperator.hpp Client.hpp EventHandler.hpp Server.hpp utils.hpp SystemCalls.hpp Hasher.hpp Exceptions.hpp irc.hpp Logger.hpp ReplyCodes.hpp Content.hpp)
OBJS = $(SRCS:.cpp=.o)
INCLUDES = srcs/headers

LEAK_REPORT = leaks.log

CC = c++
VERSION = 98
CFLAGS = -g -Wall -Wextra -Werror -std=c++$(VERSION) -I$(INCLUDES)
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
	@valgrind $(VALGRIND_FLAGS) ./$(NAME) 8080 beetle 2> $(LEAK_REPORT)
	@echo "$(GREEN)leak report generated$(NC)"

re: fclean all

.PHONY: all clean re

.SILENT: all $(NAME) $(OBJS) clean fclean re

.IGNORE: clean fclean