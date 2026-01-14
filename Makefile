NAME		:= minishell
CC		:= cc
CFLAGS		:= -Wall -Wextra -Werror
INCLUDES	:= -Iinclude

SRCS		:= \
	srcs/main.c \
	srcs/utils.c \
	srcs/tokenization.c \
	srcs/token_utils.c

OBJS		:= $(SRCS:.c=.o)

# Readline lives in a separate library, so we must explicitly link it.
# Some distros also require history + ncurses/tinfo depending on how readline was built.
READLINE_LIBS	:= -lreadline -lhistory -lncurses

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(READLINE_LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f include/*.gch

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
