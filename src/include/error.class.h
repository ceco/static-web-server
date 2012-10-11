/**
 * @name error.class.h
 * @version 0.1
 * @author Tsvetan Filev <tsvetan.filev@gmail.com>
 * @date 09/28/2012
 * 
 * This header file contains the definitions of class Error
 * Error is used to provide information, show warning and error messages and to terminate the program if fatal error occurs
 * 
 */
#ifndef _ERROR_HEADER_

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define _ERROR_HEADER_ 1

    class Error {
        string now();
        
        public:
            void info(string message);
            void warning(string message);
            void error(string message);
            void fatal(string message);
            string itostr(int);

    }; // end class Error

#endif