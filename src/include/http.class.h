/**
 * @name http.class.h
 * @version 0.1
 * @author Tsvetan Filev <tsvetan.filev@gmail.com>
 * @date 09/28/2012
 * 
 * This header file contains the definitions of class Http
 * Http is used to generate http headers and parse http requests
 * 
 */
#ifndef _HTTP_HEADER_

#include <string>
#include <stdio.h>

using namespace std;

#define _HTTP_HEADER_ 1

    class Http {

        public:
            string file_not_found(const string& proto);
            string header(const string& proto, const string& mimetype, unsigned long size);
            string filename(const string& request);
            string protocol(const string& request);
    };

#endif