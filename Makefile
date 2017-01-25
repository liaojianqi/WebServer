CC = g++

default:
	make Test
request.o: request.h request.cpp
	$(CC) -c request.cpp
response.o: response.h response.cpp
	$(CC) -c -lboost_system response.cpp
server.o: server.h server.cpp
	$(CC) -c -lboost_system server.cpp
lib: request.o response.o server.o
	make request.o
	\make response.o
	\make server.o
Test: Test.cpp request.o response.o server.o
	$(CC) -lboost_system Test.cpp request.o response.o server.o -o Test
run: Test
	./Test
clean:
	rm request.o response.o server.o Test
