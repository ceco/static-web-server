#include "file.class.h"

int main(){
  file file1 ("share.png"); // read file
  ofstream file2 ("share1.png", ios::out|ios::binary); // write file
  
  if (file1.is_open() && file2.is_open())
  {
      cout << "Mime is: " << file1.mime_type() << "\n";
      cout << "Size is: " << file1.length() << "\n";
      while ( file1.read() )
        file2.write( file1.buffer(), file1.bufferLength() );
  }
  else 
      cout << "Unable to open files" << "\n";
  
  file2.close();
  
  return 0;
}