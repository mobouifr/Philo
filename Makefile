NAME = philo

SRC = philo.c \
		philo_utils.c \
		philo_utils2.c \
		monitor.c \
		routine.c \
		./utils/ft_atoi.c \
		./utils/ft_isdigit.c

OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -pthread #-fsanitize=thread

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o:%.c
	$(CC) $(CFLAGS)  -c $< -o $@

clean : 
	rm -rf $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.SECONDARY : $(OBJ)