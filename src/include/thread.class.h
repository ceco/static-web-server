/**
 * @name thread.class.h
 * @version 0.1
 * @author Tsvetan Filev <tsvetan.filev@gmail.com>
 * @date 09/28/2012
 * 
 * This header file contains the definitions of class Thread
 * Thread is used to start asynchronous threads 
 * 
 */
#ifndef _THREAD_HEADER_

#include <pthread.h>
#include <iostream>
#include "error.class.h"

using namespace std;

#define _THREAD_HEADER_ 1

    class Thread {
        Error& error;

        public:
            Thread(Error& err);
            int async(void * (*start_routine)(void *), void *arg);
    };

#endif