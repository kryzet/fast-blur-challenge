CC := clang

CFLAGS := -march=native -std=c18 -Wall -Wextra -Wpedantic -pthread
CFLAGS_debug := -O0 -ggdb3 -gdwarf-4
CFLAGS_release := -O3 -DNDEBUG

LDFLAGS := -lm

all: debug

debug: CFLAGS += $(CFLAGS_debug)
debug: filter
release: CFLAGS += $(CFLAGS_release)
release: filter

filter: filter.c filter.h blur.c
	$(CC) $(CFLAGS) -o $@ filter.c $(LDFLAGS)

.PHONY: release debug

###############################################################################

# The following is not pretty but should get the job done

# Presumes you have an images/ directory in the project root containing an
# image '3x3.bmp'. For now you'll have to change the path manually to bench
# using another image.
# Set N to be the number of runs per benchmark.
# Each benchmark will output to it's own file based on the source file name.
N := 10
bench_files := $(wildcard submissions/*)
bench-all: | bench submissions
	@$(foreach f, $(bench_files), \
        tput setaf 3 && echo "Benching $f..." && tput sgr0 \
        && cp $f bench/blur.c && cd bench \
        && MAKEFLAGS=--no-print-directory $(MAKE) release \
        && if [[ -f filter ]]; then tput setaf 6 ; \
            ./filter ../images/3x3.bmp out_$(basename $(notdir $f)).bmp $(N) ; tput sgr0 ; \
        fi \
        ; cd .. \
        ; \
    )

bench:
	mkdir -p $@
	cp filter.c filter.h Makefile stb_image.h $@

submissions:
	mkdir -p $@
	@echo "Place one or more versions of 'blur.c' into submissions/"

clean-bench:
	rm -rf bench/

.PHONY: bench-all clean-bench
