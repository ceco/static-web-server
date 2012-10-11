/**
 * @name http.class.cpp
 * @version 0.1
 * @author Tsvetan Filev <tsvetan.filev@gmail.com>
 * @date 09/28/2012
 * 
 * This code file contains the declaration of class Http
 * Http is used to generate http headers and parse http requests
 * 
 */
#include "http.class.h"

/**
 * @name file_not_found
 * @param const string& reference to protocol string
 * @return string http headers
 * 
 * Generates headers and content in case of file not found on server
 * 
 */ 
string Http::file_not_found(const string& proto){
    string response = "HTTP/";
    response += proto;
    response += " Not Found\r\n";
    response += "Server: MediaWebServer/1.0 (Linux)\r\n";
    response += "Vary: accept-language,accept-charset\r\n";
    response += "Accept-Ranges: bytes\r\n";
    response += "Content-Type: text/html; charset=iso-8859-1\r\n";
    response += "Content-Language: en\r\n";
    response += "Content-Length: 26\r\n";
    response += "Connection: close\r\n\r\n";
    response += "<b>404 File not found!</b>";
    
    return response;
} // end file_not_found

/**
 * @name header
 * @param const string& reference to protocol string
 * @param const string& reference to mime type string
 * @param unsigned long size of file in bytes
 * @return string headers for http response
 * 
 * Generates headers for response in case of file exists
 * Set file expire date 10 days later so browsers can cash it
 * 
 */
string Http::header(const string& proto, const string& mimetype, unsigned long size){
    char size_buffer[10];
    char expires_buffer[30];
    string months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    string days[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    time_t rawtime;
    struct tm * ptm;

    // Get size
    sprintf(size_buffer, "%lu", size);
    string size_string = size_buffer;
    
    // Get time
    time ( &rawtime );
    rawtime += 864000; // add 10 days
    ptm = gmtime ( &rawtime );
    sprintf(expires_buffer,"%s, %02d %s %04d %02d:%02d:%02d", days[ptm->tm_wday].c_str(), ptm->tm_mday, months[ptm->tm_mon].c_str(), 1900 + ptm->tm_year, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    string expires_string = expires_buffer;
    
    string response = "HTTP/";
    response += proto;
    response += " 200 OK\r\n";
    response += "Content-Type: ";
    response += mimetype;
    response += "\r\n";
    response += "Content-Length: ";
    response += size_string;
    response += "\r\n";
    response += "Expires: ";
    response += expires_string;
    response += " GMT\r\n";
    response += "Connection: close\r\n\r\n";
    
    return response;
} // end header

/**
 * @name filename
 * @param const string& reference to request string
 * @return string path of requested file
 * 
 * Extracts filename from HTTP GET request.
 * @example
 * GET /media/image.png HTTP/1.1
 * File is ./media/image.png
 *
 * GET media/image.png HTTP/1.1
 * File is media/image.png
 * 
 */
string Http::filename(const string& request){
    size_t start,end;
    string filename;

    if( request.length() > 0 ){
        start = request.find("GET ");
        end = request.find(" HTTP/1.");
        if( start != std::string::npos && end != std::string::npos ){
            filename = request.substr(start + 4, end - start - 4);
            if( filename[0] == '/' ) filename.insert(0, ".");
        }
    }

    return filename;
} // end filename

/**
 * @name protocol
 * @param const string& reference to request string
 * @return string HTTP protocol version
 * 
 * Extracts protocol version from GET query
 * 
 * @example 
 * 1.0 
 * 1.1
 * 
 */
string Http::protocol(const string& request){
    size_t start;
    string proto = "1.1";
    
    if( request.length() > 0 ){
        start = request.find(" HTTP/1.");
        if( start != std::string::npos )
            proto = request.substr(start + 6, 3);
    }

    return proto;
} // end protocol
