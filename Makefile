CC := clang

CFLAGS := -march=native -std=c18 -Wall -Wextra -pthread
CFLAGS_debug := -O0 -ggdb3 -gdwarf-4
CFLAGS_release := -O3

LDFLAGS := -lm

all: debug

debug: CFLAGS += $(CFLAGS_debug)
debug: filter
release: CFLAGS += $(CFLAGS_release)
release: filter

filter: filter.c filter.h
	$(CC) $(CFLAGS) -o $@ filter.c $(LDFLAGS)

.PHONY: release debug
