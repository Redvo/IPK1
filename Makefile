#
# IPK Project 1 - 18.3.2015
# Makefile
# Author: Lukáš Rendvanský
# E-mail: xrendv00@stud.fit.vutbr.cz
#	

# Names of output executable files

NAME_CLIENT=client
NAME_SERVER=server

# Names of object files

OBJECT_CLIENT=$(NAME_CLIENT).o arguments.class.o connection.class.o exception.class.o searcher.class.o
OBJECT_SERVER=$(NAME_SERVER).o arguments.class.o connection.class.o exception.class.o searcher.class.o

# Names of header files

HEADER_CLIENT=arguments.class.hpp connection.class.hpp exception.class.hpp searcher.class.hpp
HEADER_SERVER=arguments.class.hpp connection.class.hpp exception.class.hpp searcher.class.hpp

# Compiler

CXX=g++

# Compiler flags

CXXFLAGS=-Wall -Wextra -pedantic -g -W -O3

# Start rule

all: $(NAME_CLIENT) $(NAME_SERVER)

# Universal rule to generate obejct files	

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking required object files to runnable binary files

$(NAME_CLIENT): $(OBJECT_CLIENT)
	$(CXX) $(CXXFLAGS) $(OBJECT_CLIENT) -o $@

# Linking required object files to runnable binary files

$(NAME_SERVER): $(OBJECT_SERVER)
	$(CXX) $(CXXFLAGS) $(OBJECT_SERVER) -o $@

# Removing all object and binary files

rmall: pack clean
	rm -f *.c *.h protokol.pdf

clean:
	rm -f *~ *.o 
	rm -f $(NAME_CLIENT) $(NAME_SERVER)

pack:
	zip -r xrendv00.zip *.cpp *.hpp protokol.pdf Makefile

# End of Makefile