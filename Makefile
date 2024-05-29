CC:=g++

CFLAGS:=-std=c++11 -Wall -I/usr/local/include -Iinclude -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

SRCS:=include/glad/glad.c yaml.cpp main.cpp
TEST_SRCS:=include/glad/glad.c test.cpp

OBJS:=$(SRCS:.cpp=.o)
TEST_OBJS:=$(TEST_SRCS:.cpp=.o)

EXEC:=visualize
TEST := test

.PHONY: all clean FORCE

all: FORCE $(EXEC) $(TEST)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ /usr/local/include/libglfw3.a

$(TEST): $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ /usr/local/include/libglfw3.a

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

test.o: test.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

