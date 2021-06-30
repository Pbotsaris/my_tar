SRC =	src/my_tar.c    		\
		src/create_header.c	 	\
		src/my_itoa.c 			\
		src/archive.c			\
		src/option.c			\
		src/lstar.c

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

