NAME            = a.out

CC              = gcc

SRC             = src/main.c \
                  src/simulator.c

OBJ             = $(SRC:.c=.o)

DEP             = $(SRC:.c=.d)

INCLUDES        = -I ./include/

LIBS            = -L ./lib/ -lm -lpthread -ldl -lX11 -lXext -lXrender -lXtst -O3 -no-pie

RM              = rm -rf

all: $(NAME)

-include $(DEP)

%.o: %.c
	$(CC) -MMD -MP $(INCLUDES) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LIBS)

clean:
	$(RM) $(OBJ) $(DEP)

fclean: clean
	$(RM) $(NAME)

re: fclean all
