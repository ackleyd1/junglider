PROJ=junglider

CC=gcc
CFLAGS= -Wall -g

vpath %.c src

.PHONY: all clean
all: build/$(PROJ)

build/$(PROJ): main.c utils.c
ifeq ("$(wildcard build)", "")
	@echo "creating build directory"
	@mkdir build
endif
	$(CC) $(CFLAGS) -o $@ $^ -I include -I /usr/local/src/libglvnd/include -L /opt/nvidia/lib -lGLEW -lGL -lGLU -lglut

clean:
	rm -rf build