#include "server.class.h"

int main(void){
    
    Server server;
   
    return 0; 
}

/*
int main(int argc, char *argv[])
{
  ifstream in, in2;
  ofstream out;
  
  if (argc > 1)
    out.open(argv[1]);
  else {
    out.copyfmt(cout);                                  //1
    out.clear(cout.rdstate());                          //2
    out.basic_ios<char>::rdbuf(cout.rdbuf());           //3
  }
  // output to out, for example
  out << "Hello world!" << endl;
  
  string line;
  in.open(argv[1]);
  
    while ( in.good() )
    {
      getline (in,line);
      cout << line;
    }
  //  in.close();
  
    
    
  in2.copyfmt(in);                                  //1
  in2.clear(in.rdstate());                          //2
  in2.basic_ios<char>::rdbuf(in.rdbuf());           //3
  //in2.clear();
  in2.seekg (0, ios::beg);

  cout << in.is_open() << in2.is_open() << in.rdbuf()->is_open();
  
      while ( in2.good() )
    {
      getline (in2,line);
      cout << line;
    }

}
*/

/*
//#include <numeric>
//#include <algorithm>

//int get_key(const string& str){ return accumulate(str.begin(), str.end(), 0); }

int main(int argc, char* argv[])
{
    map<string, int> files;

    //string input = argv[1] != 0 ? argv[1] : "";
    //string val = string(input);
    //int key = get_key(val);
    string input = "test";
    
    int a = files[input];
    
    bool there = file.find(input) != files.end() ? true : false;
    
    cout << "Key: " << input << " Val: " << files[input] << " There " << (there ? "yes" : "no") << endl;
    
    return 0;
}
*/

