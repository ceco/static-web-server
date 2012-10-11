#ifndef _FILE_HEADER_

#include <strings.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "error.class.h"

using namespace std;

#define _FILE_HEADER_ 1

#define FILE_BUFFER_LENGTH 1024

    class File {

        Error&        error;
        ifstream      stream;
        string        path;
        struct stat   file_stat;
        string        _mime_type;
        
        char          _buffer[FILE_BUFFER_LENGTH];
        int           _bufferLength;

        void get_mime_type();
                    
        public:
            File(Error& err);
            File(string file_path, Error& err);
            File(const File&);

            File& operator=(const File&);
            bool open();
            bool open(string file_path);
            bool is_open();
            void reset();
            int  read();
            const char* buffer();
            int  bufferLength();
            string mime_type();
            unsigned long length();
            time_t mtime();
            void close();
            
            ~File();

    }; // end class File

#endif