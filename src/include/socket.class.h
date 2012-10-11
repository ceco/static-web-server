/**
 * @name socket.class.h
 * @version 0.1
 * @author Tsvetan Filev <tsvetan.filev@gmail.com>
 * @date 09/28/2012
 * 
 * This header file contains the definitions of class Listener and class Client
 * Listener is used to bind socket listener that accepts connections
 * Client is used to establish tcp client connections
 * 
 */
#ifndef _SOCKET_HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/sendfile.h>
#include "error.class.h"
#include "file2.class.h"

using namespace std;

#define _SOCKET_HEADER_ 1

#define SOCKET_PORT 3000
#define MAXIMUM_NUMBER_OF_SOCKETS 1000
#define SOCKET_BUFFER_LENGTH 1024

    /// Listener ///
    class Listener {

        int port;  
        int sock;
        Error& error;

        public:
            
            /* listening socket */
            Listener(Error& err);
            void listen(bool non_blocking);
            void listen();
            int socket();
            void close();
                
    }; // end class Listener

    /// Client ///
    class Client {

        int sock;
        Error& error;
        
        void announce_client(struct in_addr* addr);
            
        public:
            Client(Error& err);
            Client(int _sock, Error& err);
            int connect(int listen_sock);
            
            int socket();
            string read ();
            int write(string response);
            int write(const char* response, int length);
            void close();
            
            // Inline operators
            inline Client& operator<<(const string& text){
                write( text );
                return *this;
            }
            
            inline Client& operator<<(File& file){
                off_t offset = 0;
                sendfile(sock, file.get_fd(), &offset, file.length());
                return *this;
            }
            
            inline Client& operator>>(string& text){
                text = read();
                return *this;
            }

    }; // end class Client
    
#endif