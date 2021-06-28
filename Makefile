SRC =	src/my_tar.c    		\
		src/file_handling.c	 	\
		src/my_itoa.c 			\
		src/option.c	

OBJ = $(SRC:.c=.o)

CFLAGS += -W -g3 

CC = gcc -g -o

NAME = my_tar

RM = rm 

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(NAME) $(OBJ) $(CFLAGS)

clean:
	$(RM) $(OBJ)
	$(RM) $(NAME)

