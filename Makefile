TARGET ?= glcube

CXX ?= g++
CC ?= gcc
AS ?= as

DEBUGFLAGS ?= -DDEBUG -g -O0
RELEASEFLAGS ?= -DNDEBUG -O2 -s

CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic `pkg-config --cflags gl glew glfw3 cglm`
CFLAGS ?= -std=c17 -Wall -Wextra -pedantic `pkg-config --cflags gl glew glfw3 cglm`
LDFLAGS ?= `pkg-config --libs gl glew glfw3 cglm`

SRC_DIR ?= ./src
BUILD_DIR ?= ./build
BIN_DIR ?= ./bin

all: release
########################## DONT EDIT BELOW THIS LINE ##########################

.PHONY: all debug release clean

SRCS := $(shell find $(SRC_DIR) -name *.c -or -name *.cpp -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_DIRS_FLAGS := $(addprefix -I,$(INC_DIRS))

BUILD_SRC_DIRS := $(addprefix $(BUILD_DIR)/,$(SRC_DIR))

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: CFLAGS   += $(DEBUGFLAGS)
debug: $(BIN_DIR)/$(TARGET)

release: CXXFLAGS += $(RELEASEFLAGS)
release: CFLAGS   += $(RELEASEFLAGS)
release: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c | $(BUILD_SRC_DIRS)
	$(CC) $(CFLAGS) $(INC_DIRS_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp | $(BUILD_SRC_DIRS)
	$(CXX) $(CXXFLAGS) $(INC_DIRS_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.s.o: %.s | $(BUILD_SRC_DIRS)
	$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/%.d: % | $(BUILD_SRC_DIRS)
	@$(CC) $< -MM -MT $(@:%.d=%.o) >$@

$(BUILD_DIR) $(BUILD_SRC_DIRS) $(BIN_DIR):
	@$(MKDIR_P) $@

clean:
	$(RM_RF) $(BUILD_DIR) $(BIN_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
RM_RF ?= rm -rf

