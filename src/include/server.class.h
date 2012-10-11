/**
 * @name server.class.h
 * @version 0.1
 * @author Tsvetan Filev <tsvetan.filev@gmail.com>
 * @date 09/28/2012
 * 
 * This header file contains the definitions of class Server
 * Server is used to handle program interrupts (Ctrl+C, kill), create listener socket, 
 * accept user connections and start threads that serve the content
 * 
 */
#ifndef _SERVER_HEADER_

#include <signal.h>
#include <map>
#include "error.class.h"
#include "socket.class.h"
#include "http.class.h"
#include "thread.class.h"
#include "file2.class.h"

#define _SERVER_HEADER_ 1
#define MAXIMUM_NUMBER_OF_CLIENTS 1000
#define MAXIMUM_FILES_OPEN 1000

    typedef map <string,File>::iterator files_iterator;
        
    class Server {
        Error error;
        Listener listener;
        Http http;
        Thread thread;

        Client client;
        
        const int max_files_open;
        const string defualt_filename;
        map <string, File> files;
        map <int,int> sockets;
        files_iterator default_it;

        static Server *serverPtr;

        void terminate();
        static void sighandler(int sig);
        static void* handle_client(void* server_ptr);
    
        public:
            
            Server();

    }; // end class Server
    
#endif