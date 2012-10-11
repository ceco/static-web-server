#include "file.class.h"

File::File(Error& err) : _bufferLength(0), error(err) {}
File::File(string file_path, Error& err) : path(file_path), _bufferLength(0), error(err) {} 
File::File(const File& file) : error(file.error) { }

File& File::operator=(const File& f2){

    this->stream.copyfmt(f2.stream);
    this->stream.clear(f2.stream.rdstate());
    this->stream.basic_ios<char>::rdbuf(f2.stream.rdbuf());
    this->stream.seekg(0, ios::beg);
    
    return *this;
} // end operator=

bool File::open(){
    // open file
    stream.open(path.c_str(), ios::in|ios::binary);
    
    // get mime type and size
    if( stream.is_open() ){
        //error.info("Opening file " + path);          
        get_mime_type();
        stat(path.c_str(), &file_stat);
    }
    else
        error.error("Error opening " + path);
    
    return stream.is_open();
} // end open

bool File::open(string file_path){
    if( path != file_path ){
        close();
        path = file_path;
        return open();
    }
    return stream.is_open();
} // end open

bool File::is_open(){
    return stream.is_open();
} // end is_open

void File::reset(){
    if( stream.is_open() ){
        stream.clear();
        stream.seekg (0, ios::beg);
    }
}

int File::read(){
    //if (stream.is_open())
    //{
        bzero(_buffer, sizeof(_buffer));
        if( stream.read( _buffer, FILE_BUFFER_LENGTH ) ) _bufferLength = FILE_BUFFER_LENGTH;
        else _bufferLength = stream.gcount();
    //}

    return _bufferLength;
} // end read

const char* File::buffer(){
    return _buffer;
} // end bufferString

int File::bufferLength(){
    return _bufferLength;
} // end getBufferLength

unsigned long File::length(){
    if( !stream.is_open() ) return 0;
    return file_stat.st_size;
} // end length

string File::mime_type(){
    if( !stream.is_open() ) return "";
    return _mime_type;
} // end mime_type

time_t File::mtime(){
    if( !stream.is_open() ) return 0;
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
    //if( stream.is_open() ){
        stream.close();
        //stream.clear();
       // error.info("Closing file " + path);
    //}    
}

File::~File(){
    close();
} // end destructor
