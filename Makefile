NAME	=	Pestilence
SRCS	=	main.asm
OBJS	=	$(SRCS:.asm=.o)
CC		=	gcc
AS		=	nasm
CFLAGS	=	-Wall -Wextra -Werror -no-pie -Wl,--strip-all
AFLAGS	=	-felf64
RM		=	rm -rf

all		:	$(NAME)

$(NAME)	:	$(OBJS)
			$(CC) $(CFLAGS) $^ -o $@

%.o		:	%.asm
			$(AS) $(AFLAGS) $< -o $@

clean	:
			$(RM) $(OBJS)

fclean	:	clean
			$(RM) $(NAME)

re		:	fclean all
