# gffk makefile
#  Jan Piotr Buchmann <jpb@members.fsf.org>
#  copyright BDS-HHU 2026


GIT_COMMIT := $(shell git describe --always --dirty --match 'NOT A TAG')

MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
SRC_DIR      := $(MAKEFILE_DIR)../src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
TARGET  := $(BIN_DIR)/gfftk
INCLUDES := -I$(SRC_DIR)

CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic  $(INCLUDES) -DVERSION_NUMBER='"$(GIT_COMMIT)"'
LDFLAGS  :=

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))


BUILD ?= release

ifeq ($(BUILD),debug)
    CXXFLAGS += -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
    LDFLAGS  += -fsanitize=address,undefined
else ifeq ($(BUILD),prof)
    CXXFLAGS += -O2 -pg
    LDFLAGS  += -pg
else
    CXXFLAGS += -O2 -DNDEBUG
endif


.PHONY: all
all: $(TARGET)

# Link
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX)	$(CXXFLAGS)	$^	-o	$@	$(LDFLAGS)
	@echo "	LD	$@	[$(BUILD)]"

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "  CXX $<"


#Auto-generated dependency files
-include $(OBJS:.o=.d)

# Create output directories
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Debug build shortcut
.PHONY: debug
debug:
	$(MAKE) BUILD=debug

# Remove compiled objects and binary
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	@echo "  Cleaned."

# Debug makefile
.PHONY: info
info:
	@echo "VERSION  : $(GIT_COMMIT)"
	@echo "TARGET   : $(TARGET)"
	@echo "SRCS     : $(SRCS)"
	@echo "OBJS     : $(OBJS)"
	@echo "CXXFLAGS : $(CXXFLAGS)"
	@echo "LDFLAGS  : $(LDFLAGS)"
	@echo "BUILD    : $(BUILD)"
