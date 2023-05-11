CC = g++ -std=c++11 -Wall -Wextra -g
CFLAGS = -c

client: client.o requests.o buffer.o helper.o
	$(CC) $^ -o $@

client.o: client.cpp
	$(CC) $(CFLAGS) $< -o $@

request.o: requests.cpp
	$(CC) $(CFLAGS) $< -o $@

buffer.o: buffer.cpp
	$(CC) $(CFLAGS) $< -o $@

helper.o: helper.cpp
	$(CC) $(CFLAGS) $< -o $@

run: client
	./client

clean:
	rm -f *.o client
