CC=g++
CFLAGS=-I.

client: client.cpp requests.cpp helpers.cpp buffer.cpp functions.cpp
	$(CC) -o client client.cpp requests.cpp helpers.cpp buffer.cpp functions.cpp -Wall

run: client
	./client

clean:
	rm -f *.o client
