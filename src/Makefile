# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
#  -O2		- turns on optimization flags
#  -l...  - library dependencies
#  -I...	- include folders 
CFLAGS  = -g -Wall -O2 -lcurl -ljsoncpp -lsqlite3 -I../include

# Target and dependencies
TARGET = main
OBJS   = main.o ../include/api.o

# Build and run 
all: $(TARGET)
		./$(TARGET)

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
		$(CC) $(CFLAGS) -c -o $@ $<

# Cleanup 
clean:
	$(RM) $(TARGET)
