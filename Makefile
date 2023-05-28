NAME = philo

SRC_DIR = ./src

BIN_DIR = ./bin/

SRC = $(SRC_DIR)/philosophers.c		\
	  $(SRC_DIR)/init.c				\
	  $(SRC_DIR)/philo_functions.c	\
	  $(SRC_DIR)/atoitoa.c			\
	  $(SRC_DIR)/threads.c			\
	  $(SRC_DIR)/utils.c			\

OBJ = $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)%.o)


INCLUDES = ./includes

CFLAGS = -Wall -Werror -Wextra -I$(INCLUDES) -g3

all :	$(BIN_DIR) $(NAME)

$(OBJ) :	 $(BIN_DIR)%.o: $(SRC_DIR)/%.c
		@ $(CC) $(CFLAGS) -c $< -o $@

$(NAME) :	$(BIN_DIR) $(OBJ)
			@ $(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline
			@ echo "\e[33m\e[1m\tMake\e[0m [🗿] : \e[1mDone ! ✅"

$(BIN_DIR) :
		@ mkdir -p $(BIN_DIR)

clean :
		@ rm -f $(OBJ)
		@ rm -rf $(BIN_DIR)
		@echo "\e[33m\e[1m\tMake\e[0m [🗿] : \e[1mRemove binary files .. 🧹"

fclean : clean
		@ rm -f $(NAME)
		@echo "\e[33m\e[1m\tMake\e[0m [🗿] : \e[1mRemove executable .. 🗑️"
		
re :
	@echo "\e[33m\e[1m\tMake\e[0m [🗿] : \e[1mRecompile .. 🔄"
	@ $(MAKE) -s fclean all

.PHONY: all clean fclean re
