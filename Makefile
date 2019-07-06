SUFFIXES += .d
NODEPS := clean distclean

TARGET := rbgPlayer
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
BIN_DIR := bin
DEP_DIR := dep
GEN_DIR := gen
REASONER := reasoner
MAIN_FILE := $(SRC_DIR)/main.cpp

C := g++
INCLUDE := -I$(INC_DIR) -I$(GEN_DIR)
COMMON_CFLAGS = -Wall -Wextra -Wpedantic -Ofast -march=native -flto -std=c++17 -pthread
CFLAGS := $(COMMON_CFLAGS) -s $(INCLUDE)

OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
OBJECTS := $(OBJ_DIR)/$(REASONER).o $(OBJECTS)
DEPFILES := $(patsubst $(SRC_DIR)/%.cpp, $(DEP_DIR)/%.d, $(wildcard $(SRC_DIR)/*.cpp))
DEPFILES := $(DEP_DIR)/$(REASONER).d $(DEPFILES)

all: $(TARGET)

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEPFILES)
endif

$(OBJ_DIR)/%.o: $(GEN_DIR)/%.cpp $(DEP_DIR)/%.d | $(OBJ_DIR)
	$(C) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d | $(OBJ_DIR)
	$(C) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%.d: $(GEN_DIR)/%.cpp | $(DEP_DIR)
	$(C) $(CFLAGS) -MM -MT '$(patsubst $(GEN_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<) $@' $< -MF $@

$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp | $(DEP_DIR)
	$(C) $(CFLAGS) -MM -MT '$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<) $@' $< -MF $@

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(C) $(CFLAGS) $(OBJECTS) -o $(BIN_DIR)/$@

$(DEP_DIR):
	mkdir -p $(DEP_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(DEP_DIR)

distclean: clean
	rm -rf $(BIN_DIR)
