SRC =	my_tar.c    	\
		file_handling.c \
		my_itoa.c 		\
		option.c	

OBJ = $(SRC:.c=.o)

CFLAGS += -W -Wno-implicit-fallthrough -g3 

CC = gcc -g -o

NAME = my_tar

RM = rm 

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(NAME) $(OBJ) $(CFLAGS)

clean:
	$(RM) $(OBJ)
	$(RM) $(NAME)

