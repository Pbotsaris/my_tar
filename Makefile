OBJ=obj/my_tar.o    	   				\
		obj/header.o	      			 	\
		obj/utils.o         			  \
		obj/debug.o          			  \
		obj/archive_and_append.o		\
		obj/option.o	    					\
		obj/list_and_extract.o

CFLAGS=-W -g3 -Iinclude
HEADER=include/my_tar.h
CC=gcc 
BIN=bin/my_tar
RM=rm -rf

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(CFLAGS) 

obj/%.o: src/%.c 
	${CC} ${CFLAGS} -c $< -o $@

clean:
	$(RM) bin/my_tar bin/*.dSYM obj/*
