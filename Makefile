# Folder/directory names
OBJ_DIR := obj
SRC_DIR := src

# List of source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

.default: all

all: mazeRunner

# Cleans mazeRunner and object files
clean:
	rm -f mazeRunner $(OBJS)

# Creates mazeRunner executable with object files
mazeRunner: $(OBJS)
	g++ -Wall -Werror -std=c++17 -g -O -o $@ $^ -lmcpp

# Creates object files from source directory (.cpp files)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -Wall -Werror -std=c++17 -g -O -c $^ -o $@