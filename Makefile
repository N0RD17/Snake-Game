# Compiler
CC = g++


# C++ Extension Files
CXX_FILES = *.cpp

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -pedantic

# Compiler Libraries
CLIBS = -lSDL2

# Output
COUT = SnakeGame



all: ${CXX_FILES}
	${CC} ${CFLAGS} ${CXX_FILES} -o ${COUT} ${CLIBS}
