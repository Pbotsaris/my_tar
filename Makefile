OBJ=obj/my_tar.o    	   	\
		obj/create_header.o	 	\
		obj/utils.o           \
		obj/debug.o           \
		obj/archive.o		    	\
		obj/option.o	    		\
		obj/ls_tar.o

CFLAGS=-W -g 
CC=gcc 
BIN=bin/my_tar
RM=rm -r

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(CFLAGS)

obj/%.o: src/%.c
	${CC} ${CFLAGS} -c $< -o $@

clean:
	$(RM) bin/my_tar obj/*

