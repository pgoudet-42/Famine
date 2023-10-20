SRCS = ./assembly/Famine.s
NAME = Famine
FLAGS_C = -masm=intel -Wno-int-to-pointer-cast -nostartfiles -fPIC  -nostdlib -fno-builtin
FLAGS_ASM = -f elf64


OBJS = ${SRCS:.s=.o}

%.o : %.s
		@nasm ${FLAGS_ASM} $< -o $@

${NAME}: ${OBJS}
				@gcc $(FLAGS_C) $(OBJS) -o ${NAME}

all: $(NAME)

full: $(NAME) clean

	@if [ ! -e "/tmp/test/" ]; then \
    	mkdir /tmp/test/; \
    	mkdir /tmp/test2/; \
	fi
	@cp /usr/bin/ls /tmp/test/ls
	@cp /usr/bin/ls /tmp/test2/ls
	@cp /usr/bin/test /tmp/test/test
	@cp /usr/bin/test /tmp/test2/test
	@cp /usr/bin/date /tmp/test/date
	@cp /usr/bin/date /tmp/test2/date
	@if [ -e hello ]; then \
		cp hello /tmp/test/hello; \
		cp hello /tmp/test2/hello; \
	fi
	
clean : $(OBJS)
	@rm $(OBJS)

fclean :  $(NAME)  clean
	@rm $(NAME)

re: fclean all

.PHONY: all fclean clean re full