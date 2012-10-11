all:	server

file:	file.class.cpp file.cpp file.class.h
	g++ -o file file.class.cpp file.cpp -I.

server:	src/class/error.class.cpp src/class/file2.class.cpp src/class/socket.class.cpp src/class/http.class.cpp src/class/thread.class.cpp src/class/server.class.cpp src/server.cpp src/include/error.class.h src/include/file2.class.h src/include/socket.class.h src/include/http.class.h src/include/thread.class.h src/include/server.class.h
	g++ -s -o server src/class/error.class.cpp src/class/file2.class.cpp src/class/socket.class.cpp src/class/http.class.cpp src/class/thread.class.cpp src/class/server.class.cpp src/server.cpp -lpthread -I./src/include/
#  -std=c++11

clean: 
	rm -f file server