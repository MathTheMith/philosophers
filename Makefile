# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mvachon <mvachon@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/01 12:48:23 by mvachon           #+#    #+#              #
#    Updated: 2025/10/01 12:48:26 by mvachon          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME    =   philo

HEADER    = include/philo.h

CFLAGS  =   -Wall -Wextra -Werror

SRC     =   srcs/main.c \
			srcs/setup.c \
			srcs/inits.c \
			srcs/threads.c \
			srcs/utils.c \
			srcs/routine.c \
			srcs/philo_actions.c \
			srcs/fork_utils.c \
			srcs/fork_management.c

DIR_OBJ = obj
OBJ     = $(SRC:%.c=$(DIR_OBJ)/%.o)

CC      = gcc
AR      = ar rcs

GREEN	= \033[0;32m
BLUE	= \033[0;34m
GRAY	= \033[1;30m
RED		= \033[0;31m
NC		= \033[0m

all: $(NAME)
	@echo "$(GREEN)✅ Compilation terminée !$(NC)"

$(NAME): $(OBJ)
	@echo "$(BLUE)🔧 Linking...$(NC)"
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(DIR_OBJ)/%.o: %.c $(HEADER) Makefile
	@mkdir -p $(dir $@)
	@echo "$(GRAY)🛠️  Compilation de $<$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)🧹 Suppression des fichiers objets...$(NC)"
	@rm -rf $(DIR_OBJ)

fclean: clean
	@echo "$(RED)🗑️ Suppression de l'exécutable...$(NC)"
	@rm -f $(NAME)	

re: fclean all

.PHONY: all clean fclean re bonus
