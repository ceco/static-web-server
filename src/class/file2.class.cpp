#include "file2.class.h"

File::File(Error& err) : _bufferLength(0), error(err), fd(-1) {}
File::File(string file_path, Error& err) : path(file_path), _bufferLength(0), error(err), fd(-1) {} 
File::File(const File& file) : error(file.error), fd(-1) { }

bool File::open(){
    // open file

    fd = ::open(path.c_str(), O_RDONLY);
    
    // get mime type and size
    if( fd != -1 ){
        error.info("Opening file " + path);          
        get_mime_type();
        stat(path.c_str(), &file_stat);
    }
    else
        error.error("Error opening " + path);
    
    return is_open();
} // end open

bool File::open(string file_path){
    if( path != file_path ){
        close();
        path = file_path;
        return open();
    }
    return is_open();
} // end open

bool File::is_open(){
    return (fd == -1 ? false : true);
} // end is_open

int File::read(){
    //if (stream.is_open())
    //{
        bzero(_buffer, sizeof(_buffer));
        //if( stream.read( _buffer, FILE_BUFFER_LENGTH ) ) _bufferLength = FILE_BUFFER_LENGTH;
        //else _bufferLength = stream.gcount();
    //}

    return _bufferLength;
} // end read

int File::get_fd(){ return fd; }

const char* File::buffer(){
    return _buffer;
} // end bufferString

int File::bufferLength(){
    return _bufferLength;
} // end getBufferLength

unsigned long File::length(){
    if( !is_open() ) return 0;
    return file_stat.st_size;
} // end length

string File::mime_type(){
    if( !is_open() ) return "";
    return _mime_type;
} // end mime_type

time_t File::mtime(){
    if( !is_open() ) return 0;
    return static_cast<long int> (file_stat.st_mtime);
} // end mtime

void File::get_mime_type(){

    FILE *pf;
    char command[FILE_BUFFER_LENGTH];
 
    // Execute a process listing
    sprintf(command, "file -b --mime-type '%s'", path.c_str()); 
 
    // Setup our pipe for reading and execute our command.
    pf = popen(command,"r"); 
 
    if(!pf){
      error.error("Could not open pipe for " + string(command));
      _mime_type = "";
    }
 
    // Grab data from process execution
    fgets(command, FILE_BUFFER_LENGTH , pf);
 
    if (pclose(pf) != 0)
        error.error("Failed to close command stream");
    
    _mime_type = command;
    _mime_type.erase(_mime_type.end()-1); // remove new line at end

} // end get_mime_type

void File::close(){
    if( is_open() ){
        ::close(fd);
        //stream.clear();
        error.info("Closing file " + path);
    }    
}

File::~File(){
    close();
} // end destructor
