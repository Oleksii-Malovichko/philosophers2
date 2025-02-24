NAME = philo

CC = cc
CFLAGS =  -Wall -Wextra -Werror -pthread
# CFLAGS = -fsanitize=address -g 
# CFLAGS =
SRC = srcs/philo.c srcs/utils.c srcs/monitoring.c srcs/actions.c srcs/ft_atol.c srcs/utils2.c
OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re