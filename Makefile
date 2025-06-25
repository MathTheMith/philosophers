# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/12 07:28:08 by math              #+#    #+#              #
#    Updated: 2025/06/25 05:51:30 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    =   philo

HEADER    = philo.h

CFLAGS  =   -Wall -Wextra -Werror


SRC     =   main.c \
			inits.c \
			utils.c \
			routine.c \
			philo_actions.c \
			fork_utils.c \


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

$(DIR_OBJ)/%.o: %.c $(HEADER)
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
