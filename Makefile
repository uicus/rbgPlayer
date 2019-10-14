SUFFIXES += .d
NODEPS := clean distclean
PERCENT := %

TARGET := rbgPlayer
COMPILER_DIR := rbg2cpp
COMPILER_BIN_DIR := bin
COMPILER := rbg2cpp
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
BIN_DIR := bin
DEP_DIR := dep
GEN_DIR := gen
COMMON := common
ORTHODOX_MCTS := orthodox_mcts
ORTHODOX_COMMON := orthodox_common
REASONER := reasoner

C := g++
COMMON_CFLAGS = -Wall -Wextra -Wpedantic -Ofast -march=native -flto -std=c++17 -pthread -s

define LIST_RULE
$(OBJ_DIR)/$(1)/%.o: $(3)/$(SRC_DIR)/%.cpp $(DEP_DIR)/$(1)/%.d | $(OBJ_DIR)/$(1)
	$(C) $(2) -c $$< -o $$@
$(DEP_DIR)/$(1)/%.d: $(3)/$(SRC_DIR)/%.cpp | $(DEP_DIR)/$(1)
	$(C) $(2) -MM -MT '$$(patsubst $(3)/$(SRC_DIR)/%.cpp,$(OBJ_DIR)/$(1)/%.o,$$<) $$@' $$< -MF $$@
endef

ORTHODOX_MCTS_DIRS := $(ORTHODOX_MCTS) $(ORTHODOX_COMMON) $(COMMON)
ORTHODOX_MCTS_INCLUDE := $(foreach dir,$(ORTHODOX_MCTS_DIRS),-I$(wildcard $(dir)/$(INC_DIR))) -I$(GEN_DIR)
ORTHODOX_MCTS_CFLAGS := $(COMMON_CFLAGS) $(ORTHODOX_MCTS_INCLUDE)
ORTHODOX_MCTS_OBJECTS := $(foreach dir,$(ORTHODOX_MCTS_DIRS),$(patsubst $(dir)/$(SRC_DIR)/%.cpp, $(OBJ_DIR)/$(ORTHODOX_MCTS)/%.o, $(wildcard $(dir)/$(SRC_DIR)/*.cpp)))
ORTHODOX_MCTS_OBJECTS := $(ORTHODOX_MCTS_OBJECTS) $(OBJ_DIR)/$(REASONER).o
$(foreach dir,$(ORTHODOX_MCTS_DIRS),$(eval $(call LIST_RULE,$(ORTHODOX_MCTS),$(ORTHODOX_MCTS_CFLAGS),$(dir))))
DEPFILES := $(DEPFILES) $(foreach dir,$(ORTHODOX_MCTS_DIRS),$(patsubst $(dir)/$(SRC_DIR)/%.cpp, $(DEP_DIR)/$(ORTHODOX_MCTS)/%.d, $(wildcard $(dir)/$(SRC_DIR)/*.cpp)))
$(OBJ_DIR)/$(ORTHODOX_MCTS): | $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/$(ORTHODOX_MCTS)
$(DEP_DIR)/$(ORTHODOX_MCTS): | $(DEP_DIR)
	mkdir -p $(DEP_DIR)/$(ORTHODOX_MCTS)
orthodoxMcts: $(ORTHODOX_MCTS_OBJECTS) | $(BIN_DIR)
	$(C) $(ORTHODOX_MCTS_CFLAGS) $(ORTHODOX_MCTS_OBJECTS) -o $(BIN_DIR)/$@

DEPFILES := $(DEP_DIR)/$(REASONER).d $(DEPFILES)
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEPFILES)
endif

$(OBJ_DIR)/%.o: $(GEN_DIR)/%.cpp $(DEP_DIR)/%.d | $(OBJ_DIR)
	$(C) $(CFLAGS) -c $< -o $@

$(DEP_DIR)/%.d: $(GEN_DIR)/%.cpp | $(DEP_DIR)
	$(C) $(CFLAGS) -MM -MT '$(patsubst $(GEN_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<) $@' $< -MF $@

$(DEP_DIR):
	mkdir -p $(DEP_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	cd $(COMPILER_DIR); make clean; cd ..
	rm -rf $(OBJ_DIR)
	rm -rf $(DEP_DIR)
	rm -rf $(GEN_DIR)

distclean: clean
	cd $(COMPILER_DIR); make distclean; cd ..
	rm -rf $(BIN_DIR)
