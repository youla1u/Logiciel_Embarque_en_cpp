CC=g++

# Les options du compilateur
CFLAGS=-c -Wall -std=c++17

# Les fichiers source
SRCS=$(wildcard *.cpp)

# Les fichiers objets
OBJS=$(SRCS:.cpp=.o)

all: game

game: $(OBJS)
	$(CC) $(OBJS) -o game -ljsoncpp -pthread

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

Serveur.o: Serveur.cpp
	$(CC) $(CFLAGS) Serveur.cpp -o Serveur.o -I/usr/include/jsoncpp -ljsoncpp -pthread

clean:
	rm -rf *.o myprogram
