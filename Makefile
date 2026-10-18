BUILD_DIR  ?= build
BUILD_TYPE ?= Debug

CLANG_FORMAT ?= $(shell xcrun --find clang-format 2>/dev/null || command -v clang-format)
SOURCES := $(shell find libds -name '*.c' -o -name '*.h')

.PHONY: all configure build test clean rebuild format format-check

all: build

configure:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

build: configure
	cmake --build $(BUILD_DIR)

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean build

format:
	$(CLANG_FORMAT) -i $(SOURCES)

format-check:
	$(CLANG_FORMAT) --dry-run --Werror $(SOURCES)
