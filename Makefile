# Files
NAME = philo
Libft = libft/libft.a

# Use ls -m *.c | tr -d "," | tr -d "\n" | sed "s/ ft_[a-z_]*_bonus.c//g"
Files = main.c ft_atoi.c

# Replaces the .c in each file to .o
Files_O = $(Files:.c=.o)

# Compiles every .c file into a .o file, $< referring to the prequisite while $@ the target
%.o: %.c
	cc -Wall -Wextra -Werror -c $< -o $@

$(NAME): fclean $(Files_O)
	cc -Wall -Wextra -Werror $(Files_O) -o $(NAME) 

bonus: fclean $(Files_O)
	cc -Wall -Wextra -Werror $(Files_O) -o $(NAME) 

all: $(NAME)

clean:
	rm -rf $(Files_O)
	
fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: all bonus clean fclean re