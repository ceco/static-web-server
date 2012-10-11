/**
 * @name server.class.cpp
 * @version 0.1
 * @author Tsvetan Filev <tsvetan.filev@gmail.com>
 * @date 09/28/2012
 * 
 * This code file contains the declaration of class Server
 * Server is used to handle program interrupts (Ctrl+C, kill), create listener socket, 
 * accept user connections and start threads that serve the content
 * 
 */
#include "server.class.h"

/**
 * @name serverPtr
 * 
 * static pointer used to call Server::terminate on interrupt (Ctrl+C, kill)
 * 
 */
Server *Server::serverPtr;

/**
 * @name Server
 * 
 * Constructor that runs infinite cycle
 * Starts listener socket, accepts client connections and starts asynchronous threads
 * 
 */
Server::Server(): listener(error), thread(error), client(error), max_files_open(MAXIMUM_FILES_OPEN), defualt_filename("__DEFAULT__") {
    
    // set signal handler
    serverPtr = this;
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGINT,  &sighandler);

    // bind socket listener
    listener.listen();
            
    // add default file used when number of files is greater than max_files_open
    //default_it = files.insert(pair<string,File>(defualt_filename,File(error))).first;
    
    // accept connections
    while (true) {
        client.connect(listener.socket()); // wait for connection
        if( client.socket() > 0 ){ // if connect OK
            sockets[client.socket()] = client.socket(); // safe thread socket number
            thread.async(handle_client, &sockets[client.socket()]); // and run thread handler
        }
    }

} // end constructor

/**
 * @name terminate
 * 
 * release all resources
 * 
 */
void Server::terminate(){
    // close all files
    for( files_iterator it = files.begin(); it != files.end(); it++ ) it->second.close();
    
    // close listen socket
    listener.close();
    
    error.info("Exit...");
} // end terminate

/**
 * @name sighandler
 * @param int signal number (not used)
 * 
 * Static function called on signal
 * Release all resources and return to OS
 * 
 */
void Server::sighandler(int sig)
{
    // Release resources
    serverPtr->terminate();
    
    // Return to OS
    exit(0);
} // end sighandler
    
/**
 * @name handle_client
 * @param void* pointer to thread parameter
 * @return void* pointer to thread (NULL on self terminate)
 * 
 * Handler that serves the client connection
 * Reads client request, writes response, closes socket and terminates thread on return
 * 
 */
void* Server::handle_client(void* arg_ptr) {
    string request;
    
    // terminates thread on return
    pthread_detach(pthread_self());

    // convert argument to server reference
    Server& server = *serverPtr;
    int sock = *((int*) arg_ptr);
    
    // copy server.client object to new one for this thread so that server.client can be reused for other connections
    Client client(sock, server.error);
    
    //server.error.info("Thread is running on sock " + server.error.itostr(client.socket()));

    // read HTTP request
    client >> request;
    
    // extract file name and protocol from request
    string filename = server.http.filename(request);
    string httpVersion = server.http.protocol(request);

    // try {
    
    File *file;
    bool tmp_file = false;
    files_iterator it = server.files.find(filename);
    
    // file found
    if( it != server.files.end() ) file = &it->second;
    // if too many files open create temporary
    else if( server.files.size() >= server.max_files_open ){ tmp_file = true; file = new File(server.error); }
    // add file in map
    else file = &server.files.insert(pair<string,File>(filename,File(server.error))).first->second;

    if( server.files.size() >= server.max_files_open || (file != 0 && !file->is_open()) ) file->open(filename);

/*
    files_iterator it = server.files.find(filename);
    
    // file found
    if( it != server.files.end() );
    // if too many files use default
    else if( server.files.size() - 1 >= server.max_files_open ) it = server.default_it;
    // add file in map
    else it = server.files.insert(pair<string,File>(filename,File(server.error))).first;
*/

    // send HTTP response
    if( filename.length() && file != 0 && file->is_open()) // http 200 OK
    {
        // send header
        client << server.http.header(httpVersion, file->mime_type(), file->length() );

        // send file contents
        //off_t offset = 0;
        //sendfile (client.socket(), file->get_fd(), &offset, file->length());
        client << *file;
                
        // close file if temporary
        if( tmp_file ) delete file;
    }
    else // http 404 not found
        client << server.http.file_not_found(httpVersion);

/*    
    // Send HTTP response
    if( filename.length() && file.is_open() ) // http 200 OK
    {
        // send header
        client << server.http.header(httpVersion, file.mime_type(), file.length() );
        // send file contents
        client << file;
    }
    else // http 404 not found
        client << server.http.file_not_found(httpVersion);
*/
    // close client socket
    client.close();

    return NULL; // detached thread terminates on return
} // end handle_client
