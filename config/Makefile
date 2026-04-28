##
## makefiel gfftk
##


SRC_DIR := ../src
OBJ_DIR := build
BIN_DIR := bin
TARGET  := $(BIN_DIR)/gfftk
INCLUDES := -I$(SRC_DIR)

CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic  $(INCLUDES)
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

# Remove everything including generated files
.PHONY: distclean
distclean: clean
	rm -f gprof_analysis.txt

# Show gprof report (after a prof build + run)
.PHONY: gprof
gprof:
	gprof $(TARGET) gmon.out > gprof_analysis.txt
	@echo "  Profile written to gprof_analysis.txt"

# Print effective flags / sources (useful for debugging the Makefile itself)
.PHONY: info
info:
	@echo "TARGET   : $(TARGET)"
	@echo "SRCS     : $(SRCS)"
	@echo "OBJS     : $(OBJS)"
	@echo "CXXFLAGS : $(CXXFLAGS)"
	@echo "LDFLAGS  : $(LDFLAGS)"
	@echo "BUILD    : $(BUILD)"
