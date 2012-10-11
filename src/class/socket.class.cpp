/**
 * @name socket.class.cpp
 * @version 0.1
 * @author Tsvetan Filev <tsvetan.filev@gmail.com>
 * @date 09/28/2012
 * 
 * This code file contains the declarations of class Listener and class Client
 * Listener is used to bind socket listener that accepts connections
 * Client is used to establish tcp client connections
 * 
 */
#include "socket.class.h"

/////////////////////////////////////////////////////// LISTENER ////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @name Listener
 * @param Error& reference to error object
 * 
 * Listener constructor
 * 
 */
Listener::Listener(Error& err) : port(SOCKET_PORT), error(err) {}

/**
 * @name listen
 * @param bool false for blocking socket, true for non blocking socket
 * 
 * Creates socket, reuses old sockets, binds to specific address and port and begins to listen for cleint connections
 * Exits the program if one of these functions fails
 * 
 * false: If you use 1 listener/accepter thread in your server program and 1 thread per client with simple protocol, then its much easier to use blocking sockets.
 * 
 * true: If you want an IO model where there are no threads per client but rather there is a "reactor" design pattern implementation where 1 thread (or sometimes more) 
 * serves a lot of sockets, then its easier to use non-blocking sockets.
 * 
 * For blocking sockets all recv/send/connect/accept operations are blocking until the operation either failes or succeeds. For nonblocking sockets its more complicated, 
 * but none of the previously mentioned functions block. With nonblocking sockets you usually do the read/write operations by waiting for sockets to become readable or writeable 
 * by using APIs like the crossplatform select() method, or the windows specific WSAEventSelect() or some IO completion ports if you want an MMO server. For example if 
 * the socket becomes writeable you call send() on it, and send() will return how many bytes was it able to write out. If it was able to write all bytes, you can call it again until 
 * it returns with error. In case of an error you have to check WSAGetLastError() if its WSAEWOULDBLOCK, because it means that this isn't really an error, only the send buffer of the 
 * socket (within the OS) is full, and you should wait again for the socket to become writeable. You have to check for this with almost every socket operations in case of nonblocking 
 * sockets. Connecting and accepting are also returning immediately with nonblocking sockets. In case of connection you have to wait for the socket to become writeable with select() 
 * (if you use the berkeley API) to detect when the connection becomes complete, or it will return with an exception flag set from select() in case of error. Or you can use the windows 
 * specific WSAEventSelect() that has a specific flag for connection detection. Same with accept, you have to wait for the listen socket to become readable with select(), or if you 
 * prefer windows specific API then WSAEventSelect() has special flags for testing the "accept ready" state. Maybe the select() is easier to use for the first time that the other APIs, 
 * and its also cross-platform.
 * 
 */
void Listener::listen(bool non_blocking) {
    struct sockaddr_in server_addr;
    int tr=1;
    
    // create socket
    sock = ::socket(AF_INET, SOCK_STREAM, 0);          
    if (::socket < 0) error.fatal("Problem with socket call " + error.itostr(sock));

    // kill "Address already in use" error message
    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) error.fatal("Error reusing socket " + error.itostr(sock) ); 

    // non-blocking?
    if (non_blocking) fcntl(sock, F_SETFL, O_NONBLOCK);
    
    // bind to address and port
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port); // host to network translate
    if (bind(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) error.fatal("Problem with bind call " + error.itostr(sock));

    // listen
    if (::listen(sock, MAXIMUM_NUMBER_OF_SOCKETS) < 0) error.fatal("Problem with listen call on socket " + error.itostr(sock));
    else error.info("Socket " + error.itostr(sock) + ": listening for requests on port " + error.itostr(port));

} // end listen

/**
 * @name listen
 * 
 * Start blocking socket
 * 
 */
void Listener::listen(){ listen(false); }

/**
 * @name socket
 * @return int listener socket number
 * 
 */
int Listener::socket(){ return sock; }

/**
 * @name close
 * 
 * Close current listener socket
 * 
 */
void Listener::close(){
    error.info("Closing listener socket " + error.itostr(sock));
    ::close(sock);
} // end close

/////////////////////////////////////////////////////// CLIENT ////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @name Client
 * @param Error& reference to error object
 * 
 * Client constructor
 * 
 */
Client::Client(Error& err) : error(err){}
Client::Client(int _sock, Error& err) : error(err), sock(_sock) {}

/**
 * @name announce_client
 * @param struct in_addr* pointer to an in_addr structure, defined in the netinet/in.h header file, contains only one field, an unsigned long called s_addr.
 * 
 * Anounce client connection
 * 
 */
void Client::announce_client(struct in_addr* addr) {
    char buffer[256];

    inet_ntop(AF_INET, addr, buffer, sizeof(buffer));
    error.info("Client connected from " + string(buffer) + " on sock " + error.itostr(sock));
} // end announce_client

/**
 * @name connect
 * @param int listener socket number
 * @return int client connection socket number
 * 
 * Waits for client connection
 * 
 */
int Client::connect(int listen_sock){
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);

    // accept connection
    sock = accept(listen_sock, (struct sockaddr*) &client_addr, &len);
    /*
    if (sock < 0)
        error.error("Problem accepting a client request on sock " + error.itostr(sock));
    else {
        announce_client(&client_addr.sin_addr);    
        error.info("Opening client socket " + error.itostr(sock));
    }
    */
    return sock;
} // end listen

/**
 * @name socket
 * @return int client socket number
 * 
 * Get client socket number
 * 
 */
int Client::socket(){ return sock; }
   
/**
 * @name read
 * @return string
 * 
 * Reads client request
 * 
 */
string Client::read (){
    char _buffer[SOCKET_BUFFER_LENGTH + 1];
    
    bzero(_buffer, sizeof(_buffer));
    int bytes_read = recv(sock, _buffer, sizeof(_buffer), 0); 
    if (bytes_read < 0) { error.error("Problem with recv call on sock " + error.itostr(sock)); return ""; }
    
    return string(_buffer);
} // end read

/**
 * @name write
 * @param string writes response to client
 * @return int bytes sent 
 * 
 * Send response to client. Returns -1 on error.
 * 
 */
int Client::write(string response){
    int bytes_written = send(sock, response.c_str(), response.length(), 0); 
    if (bytes_written < 0){ error.error("Problem with send call on sock " + error.itostr(sock)); return -1; }
    return bytes_written;
} // end write

/**
 * @name write
 * @param const char* buffer with data to send
 * @param int length of data
 * @return int bytes sent
 *
 * Send response to client. Returns -1 on error.
 * 
 */
int Client::write(const char* response, int length){
    int bytes_written = send(sock, response, length, 0); 
    if (bytes_written < 0){ error.error("Problem with send call on sock " + error.itostr(sock)); return -1; }
    return bytes_written;
} // end write

/**
 * @name close
 * 
 * Closes client connection
 * 
 */
void Client::close(){
    //error.info("Closing client socket " + error.itostr(sock));
    //shutdown(sock, SHUT_RDWR);
    ::close(sock);
} // end close
