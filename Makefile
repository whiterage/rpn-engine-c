CC	=	gcc
CFLAGS	=	-Wall	-Wextra	-std=c11 -g
SRC	=	src/main.c src/tokenizer.c src/rpn.c src/stack_token.c src/stack_double.c src/token_list.c src/graph.c
OBJ	=	$(SRC:.c=.o)
TARGET	=	graph

all:	$(TARGET)

$(TARGET):	$(SRC)
	$(CC)	$(CFLAGS)	-o	$@	$(SRC)	-lm

clean:
	rm	-f	$(TARGET)	*.o