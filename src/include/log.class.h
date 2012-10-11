#ifndef _LOG_HEADER_

using namespace std;

#define _LOG_HEADER_ 1

    class Log {
        ofstream access;
        ofstream error;
        ofstream debug;
        
        public:
            Log(){ }
            bool access_open();
            void access(string message){ access << message << endl; }
            bool error_open();
            void error(string message){ error << message << end; }
            bool debug_open();
            void debug(string message){ debug << message << endl; }
            ~Log(){ access.close(); error.close(); debug.close(); }
    };

#endif
