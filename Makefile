NAME = WavefunctionRenderer

CC = c++
CFLAGS = -Wall -Wextra -Werror
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system
CPPFLAGS = -MMD -MP -I. -Iinc/

SRC_DIR = src/
OBJ_DIR = obj/

RM = rm -f

SRC = main.cpp \
	WavefunctionRenderer.cpp \
	Framebuffer.cpp

OBJ = $(patsubst %.cpp, $(OBJ_DIR)%.o, $(SRC))
DEP = $(OBJ:.o=.d)

# Colors
BRED = \033[1;31m
BYEL = \033[1;33m
BGREEN = \033[1;32m
RED = \033[0;31m
YEL = \033[0;33m
GREEN = \033[0;32m
NC = \033[0m

all: $(NAME)

# Compile .cpp -> .o inside obj/
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Special case for files in root (like main.cpp if not in src/)
$(OBJ_DIR)%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

-include $(DEP)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDLIBS)
	@echo "😃 ${BGREEN}Compiled ${BYEL}$(NAME)${NC}"

clean:
	@$(RM) $(OBJ) $(DEP)
	@rm -rf $(OBJ_DIR)
	@echo "🗑️  ${BRED}Removed objects${NC}"

fclean: clean
	@$(RM) $(NAME)
	@echo "🗑️  ${BRED}Removed $(NAME)${NC}"

re: fclean all

.PHONY: all clean fclean re