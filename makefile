OBJECTS = bin/main.o bin/a_star.o bin/a_star_dependencies.o bin/node.o bin/graph.o bin/a_star_exception.o bin/path.o

CPPFLAGS = 
LDLIBS = 

CC = g++

PROGRAM_NAME = a_star

build: $(OBJECTS)
	$(CC) -Wall -std=c++11 $(CPPFLAGS) -o bin/$(PROGRAM_NAME) $(OBJECTS) $(LDLIBS)

bin/%.o: src/%.cpp
	$(CC) -Wall -std=c++11 $(CPPFLAGS) -c -o bin/$*.o src/$*.cpp $(LDLIBS)

run: build
	@bin/$(PROGRAM_NAME)

clear:
	@rm bin/*.o
	@rm bin/$(PROGRAM_NAME)
