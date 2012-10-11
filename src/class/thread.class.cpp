/**
 * @name thread.class.cpp
 * @version 0.1
 * @author Tsvetan Filev <tsvetan.filev@gmail.com>
 * @date 09/28/2012
 * 
 * This code file contains the declaration of class Thread
 * Thread is used to start asynchronous threads 
 * 
 */
#include "thread.class.h"

/**
 * @name Thread
 * @param Error& reference to error object
 * 
 * Constructor
 * 
 */
Thread::Thread(Error& err) : error(err) {}

/**
 * @name async
 * @param void * (*start_routine)(void *) routine name
 * @param void * argument to thread. If multiple arguments must be passed use struct or class
 * @return int status of the start
 *
 * Runs a thread in asynchronous mode
 * 
 */
int Thread::async(void * (*start_routine)(void *), void *arg){
    pthread_t tid;
    
    int flag = pthread_create(&tid, NULL, start_routine, arg);
    if (flag < 0) error.error("Error creating thread");
    
    return flag;
} // end async
