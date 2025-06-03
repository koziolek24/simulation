# -------------------- Variables --------------------
PYTHON      ?= $(shell which python3)
BUILD_DIR   ?= build
#CMAKE_OPTS  ?= -DENABLE_ASAN=ON
CMAKE_OPTS ?=

# -------------------- Phony targets ----------------
.PHONY: all check build run clean format test

all: build

# Dependencies ------------------------------------------------------
check:
	@echo "› Checking system dependencies…"
	@command -v pkg-config >/dev/null || { echo "pkg-config not found!"; exit 1; }
	@pkg-config --exists sfml-graphics || { echo "SFML dev libs missing!"; exit 1; }
	@$(PYTHON) -c "import importlib.util, sys; missing = [m for m in ('numpy', 'matplotlib') if importlib.util.find_spec(m) is None]; sys.stderr.write(f'Missing Python modules: {chr(44).join(missing)}\n') if missing else None; sys.exit(1) if missing else None"
	@echo "✓ All dependencies satisfied."

# Build -------------------------------------------------------------
build: check
	@echo "› Configuring (CMake)…"
	@cmake -S . -B $(BUILD_DIR) $(CMAKE_OPTS)
	@echo "› Building…"
	@cmake --build $(BUILD_DIR) -- -j$$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Run ---------------------------------------------------------------
run: build
	@echo "› Running zpr25l_sim"
	@$(BUILD_DIR)/zpr25l_sim < input.txt

# Test --------------------------------------------------------------
test: build
	@echo "› Running tests/all_tests"
	@$(BUILD_DIR)/tests/all_tests || echo "all_tests failed."

# Format ------------------------------------------------------------
format:
	@./format_code.sh || echo "clang-format target not available."

# Clean -------------------------------------------------------------
clean:
	@echo "› Removing build directory"
	@rm -rf $(BUILD_DIR)
