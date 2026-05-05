# gffk makefile
#  Jan Piotr Buchmann <jpb@members.fsf.org>
#  copyright BDS-HHU 2026


GIT_COMMIT := $(shell git describe --always --dirty --match 'NOT A TAG')

SRC_DIR := ../src
OBJ_DIR := build
BIN_DIR := bin
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

#Targets

# Debug build shortcut
.PHONY: debug
debug:
	$(MAKE) BUILD=debug

# Profile build shortcut
.PHONY: prof
prof:
	$(MAKE) BUILD=prof

# Remove compiled objects and binary
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "  Cleaned."

# Print effective flags / sources / debug makefile
.PHONY: info
info:
	@echo "VERSION  : $(GIT_COMMIT)"
	@echo "TARGET   : $(TARGET)"
	@echo "SRCS     : $(SRCS)"
	@echo "OBJS     : $(OBJS)"
	@echo "CXXFLAGS : $(CXXFLAGS)"
	@echo "LDFLAGS  : $(LDFLAGS)"
	@echo "BUILD    : $(BUILD)"
