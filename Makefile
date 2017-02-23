CC = g++

default:
	make Test
Test: Test.cpp request.h response.h server.h
	$(CC) -lboost_system Test.cpp -o Test
run: Test
	./Test
clean:
	rm Test
