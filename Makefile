# Makefile config
export WFLAGS    := -Wno-missing-braces -Wno-missing-field-initializers
export LDFLAGS   := -L$(RAYLIB_PATH)/src
export CPPFLAGS  := -I../evolution/src -I../evolution/external -I$(RAYLIB_PATH)/src -I./src
export EXEC_NAME ?= game
export CXXFLAGS  := -std=c++17 -fno-rtti -fno-exceptions

ASAN_FLAGS := -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer

BUILD_DIR := build

BUILD_MODE_PATH = $(BUILD_DIR)/$(BUILD_NAME)

ifeq ($(BUILD_MODE),RELEASE)
	CPPFLAGS   += -DNDEBUG
	CXXFLAGS   += -march=native -Ofast -s
	BUILD_NAME := release
else
	BUILD_NAME := debug
	CXXFLAGS   += -g -ggdb -O0
	export WFLAGS += -Wno-unused-parameter
ifeq ($(ASAN),1)
	export ASAN_OPTIONS := strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1
	export LDFLAGS := $(LDFLAGS) $(ASAN_FLAGS)
	CXXFLAGS       += $(ASAN_FLAGS)
	BUILD_NAME     := $(BUILD_NAME)_asan
endif
endif

export OBJ_DIR    := $(BUILD_MODE_PATH)/obj
export BUILD_PATH := $(BUILD_MODE_PATH)

EXEC_FILE := $(BUILD_MODE_PATH)/$(EXEC_NAME)

EXEC_PID = $(shell pidof $(EXEC_FILE))

.PHONY: info clean cleanall

$(EXEC_FILE):
	$(MAKE) -f Makefile.gen

all: $(EXEC_FILE)

run: all
	$(EXEC_FILE)

run_valgrind: $(EXEC_FILE)
ifeq ($(ASAN),1)
	$(error "valgrind can't run with asan")
else
	valgrind -s --leak-check=full --show-leak-kinds=all $(EXEC_FILE)
endif

run_cgdb: $(EXEC_FILE)
ifeq ($(ASAN),1)
	(ASAN_OPTIONS=sleep_before_dying=10:sleep_after_init=30 $(EXEC_FILE) > /dev/null 2>&1 &)
	sleep 5
	sudo cgdb -- -q -p $(pidof $(EXEC_FILE))
else
	cgdb $(EXEC_FILE)
endif

info:
	$(info [INFO] CXXFLAGS  : $(CXXFLAGS))
	$(info [INFO] LDFLAGS   : $(LDFLAGS))
	$(info [INFO] LDLIBS    : $(LDLIBS))
	$(info [INFO] CPPFLAGS  : $(CPPFLAGS))
	$(info [INFO] ASAN      : $(ASAN))
	$(info [INFO] BUILD_NAME: $(BUILD_NAME))
	$(info )
	$(info =====SUB MAKE INFO=====)
	$(info )
	$(MAKE) -s -f Makefile.gen info

clean:
	$(MAKE) -f Makefile.gen clean

cleanall:
	$(MAKE) -f Makefile.gen cleanall
