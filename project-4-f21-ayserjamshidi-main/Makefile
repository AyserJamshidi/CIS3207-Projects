# Ayser Jamshidi, tuk51981 - OS Project 4
# TA: Rachel Lazzaro

main: src/main.cpp
	g++ -std=c++11 src/includes/* src/helpers/* src/signalhandler/* src/main.cpp -o main -Wall -Werror -lpthread

main.o: src/main.cpp
	g++ -c main.cpp

clean:
	rm *.o main