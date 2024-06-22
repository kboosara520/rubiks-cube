APP_NAME := rubiks
COMPILER := clang++
SRC_DIR := src
BUILD_DIR := build
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Installation paths
GLFW_LOCATION := /opt/homebrew/Cellar/glfw/3.4/include/
GLFW_LIB_LOCATION := /opt/homebrew/Cellar/glfw/3.4/lib/
GLAD_LOCATION := /opt/homebrew/Cellar/glad/include/
GLM_LOCATION := /opt/homebrew/Cellar/

INCLUDES := -I$(GLFW_LOCATION) -I$(GLAD_LOCATION) -I$(GLM_LOCATION)
LINKERS := -L$(GLFW_LIB_LOCATION) -lglfw3
FRAMEWORKS := -framework OpenGL -framework IOKit -framework Cocoa # required for MacOS
SUPPRESS_WARNING := -Wno-deprecated-declarations -Wno-deprecated
CXXFLAGS := -std=c++11

# Default target builds the executable
all: $(BUILD_DIR)/$(APP_NAME)

# Make sure the directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile glad.c into glad.o
$(BUILD_DIR)/glad.o: $(SRC_DIR)/glad.c | $(BUILD_DIR)
	$(COMPILER) $< -c -o $@ -I$(GLAD_LOCATION) $(SUPPRESS_WARNING)

# Compile .cpp files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(COMPILER) $< -c -o $@ $(INCLUDES) $(SUPPRESS_WARNING) $(CXXFLAGS)

# Link and build the executable
$(BUILD_DIR)/$(APP_NAME) : $(OBJECTS) $(BUILD_DIR)/glad.o | $(BUILD_DIR)
	$(COMPILER) $^ -o $@ $(INCLUDES) $(LINKERS) $(FRAMEWORKS) $(SUPPRESS_WARNING) $(CXXFLAGS)

# builds the executable and runs it
run: all
	./$(BUILD_DIR)/$(APP_NAME)

clean-obj:
	rm -f ./$(BUILD_DIR)/*.o

clean: clean-obj
	rm -f ./$(BUILD_DIR)/$(APP_NAME)

.PHONY: clean clean-obj
