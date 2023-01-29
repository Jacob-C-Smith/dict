INCLUDES=-I./include -I/usr/local/include -I../dict/include
CC=gcc
CFLAGS= -g -fdeclspec -fms-extensions
OBJECT_DIR=./object
AS=nasm
SRCS:=$(wildcard *.c)
OBJS=$(SRCS:.c=.o) 
TARGET=dict
SLIBTARGET=lib$(TARGET).a
SOTARGET=lib$(TARGET).so

.PHONY: depend clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm *.o $(TARGET) *.a *.so

run: $(TARGET)
	./$(TARGET)

libs: $(OBJS)
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $(OBJS)
	ar -rc $(SLIBTARGET) $(TARGET).o
	$(CC) $(CFLAGS) -fPIC $(INCLUDES) -o $(SLIBTARGET) $(OBJS)
	$(CC) -shared -o $(SOTARGET) $(OBJS)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

