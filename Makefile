# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: frosa-ma <frosa-ma@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/27 18:49:22 by frosa-ma          #+#    #+#              #
#    Updated: 2022/07/30 20:40:38 by frosa-ma         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY	= all clean fclean re bonus

NAME	= monitoring
CC		= clang
CFLAGS	= -Wall -Wextra -Werror
FT		= libft.a

SRCS	= main.c
OBJSDIR	= obj
OBJS	= $(addprefix ${OBJSDIR}/, ${SRCS:%.c=%.o})

all: ${NAME}

${NAME}: ${OBJSDIR} ${OBJS}
	${CC} ${OBJS} libft/${FT} -pthread -o $@

${OBJSDIR}:
	mkdir -p $@

${OBJS}: | ${FT}

${OBJSDIR}/%.o: src/%.c
	${CC} -c $< -o $@

${FT}: | libft
	@${MAKE} bonus -C libft

libft:
	git clone https://github.com/ichmi/libft.git

clean:
	${MAKE} clean -C libft
	rm -rf ${OBJSDIR}

fclean: clean
	${MAKE} fclean -C libft
	rm -f ${NAME}

re: fclean all
