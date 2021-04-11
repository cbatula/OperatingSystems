
### Built with maker.py ###
### source = src ###
### build = build ###
### binary = prj6 ###

CC=gcc
CXX=g++

CFLAGS= -g -Wall -Wextra
CXXFLAGS= -g -Wall -Wextra -std=c++11

### GENERATED ###

default: src/main.c src/child.c
	@echo "building main..."
	@gcc -o main src/main.c src/child.c $(CFLAGS)