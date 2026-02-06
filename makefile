CC      = gcc
CFLAGS_BASE = -Wall -Wextra -Wpedantic -std=c11 -Iinclude
SANITIZERS ?=
LSAN_SUPP  = lsan.supp
CFLAGS  = $(CFLAGS_BASE) $(SANITIZERS) `sdl2-config --cflags` -Ilib
LDFLAGS = $(SANITIZERS)
LDLIBS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SRC_DIR   = src
BUILD_DIR = build
INC_DIR   = include

SRCS = main.c $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(INC_DIR)/*.h)

OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))

TARGET = $(BUILD_DIR)/game

VALGRIND_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes
VALGRIND = valgrind $(VALGRIND_FLAGS)

VALGRIND_FREE_FLAGS := --leak-check=full --show-leak-kinds=definite
VALGRIND_FREE := valgrind $(VALGRIND_FREE_FLAGS)

.PHONY: all clean run leaks test debug sanitize

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(BUILD_DIR)/main.o: main.c $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

run: all
	clear && $(TARGET)

sanitize:
	@$(MAKE) clean
	@$(MAKE) SANITIZERS="-fsanitize=address,undefined" all
	ASAN_OPTIONS=quarantine_size_mb=64 LSAN_OPTIONS=suppressions=$(LSAN_SUPP) $(TARGET)

leaks:
	@$(MAKE) clean
	@$(MAKE) SANITIZERS= LDFLAGS= all
	$(VALGRIND_FREE) $(TARGET)

clean:
	rm -rf $(BUILD_DIR)

# Test individual: make test FILE=config
# Compila y ejecuta un solo archivo .c del directorio src/
test:
ifndef FILE
	@echo "Error: Debes especificar FILE=nombre_archivo"
	@echo "Uso: make test FILE=config"
	@echo ""
	@echo "Archivos disponibles:"
	@ls -1 $(SRC_DIR)/*.c 2>/dev/null | xargs -n1 basename -s .c | sed 's/^/  - /' || echo "  (ninguno)"
	@exit 1
endif
	@mkdir -p $(BUILD_DIR)
	@echo "=== Compilando $(FILE).c ==="
	$(CC) $(CFLAGS) -DTEST_MAIN $(SRC_DIR)/$(FILE).c $(filter-out $(SRC_DIR)/$(FILE).c,$(wildcard $(SRC_DIR)/*.c)) -o $(BUILD_DIR)/test_$(FILE) $(LDLIBS)
	@echo "=== Ejecutando test_$(FILE) ==="
	@echo ""
	@$(BUILD_DIR)/test_$(FILE)

# Profiling con valgrind + kcachegrind
# Uso: make debug
debug:
	@$(MAKE) clean
	@$(MAKE) SANITIZERS= LDFLAGS= CFLAGS_BASE="$(CFLAGS_BASE) -g -O2" all
	@echo "=== Ejecutando profiling con callgrind ==="
	valgrind --tool=callgrind --callgrind-out-file=$(BUILD_DIR)/callgrind.out $(TARGET)
	@echo ""
	@echo "=== Abriendo kcachegrind ==="
	kcachegrind $(BUILD_DIR)/callgrind.out &