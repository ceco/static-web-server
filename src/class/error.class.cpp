/**
 * @name error.class.cpp
 * @version 0.1
 * @author Tsvetan Filev <tsvetan.filev@gmail.com>
 * @date 09/28/2012
 * 
 * This code file contains the declaration of class Error
 * Error is used to provide information, show warning and error messages and to terminate the program if fatal error occurs
 * 
 */
#include "error.class.h"

/**
 * @name info
 * @param string message to show
 * 
 */
void Error::info(string message){
    cout << now() << message << "\n";
} // end info

/**
 * @name warning
 * @param string warning to show
 * 
 */
void Error::warning(string message){
    cout << now() << "Warning: " << message << "\n";
} // end warning

/**
 * @name error
 * @param string error to show
 * 
 */
void Error::error(string message){
    cerr << now() << "Error: " << message << "\n";
} // end error

/**
 * @name fatal
 * @param string message to show
 * 
 * Exits the program with error message
 * 
 */
void Error::fatal(string message){
    cerr << now() << "Fatal error: " << message << "\n";
    exit(-1);
} // end fatal

/**
 * @name itostr
 * @param int integer to convert
 * @return string integer as string
 * 
 * Converts integer to string
 * 
 */
string Error::itostr(int i){
    stringstream str;
    string result;
    
    str << i;
    str >> result;
    
    return result;
} // end itostr

/**
 * @name now
 * @return string current time as string
 * 
 * Gets current timestamp
 * 
 */
string Error::now(){
    char now_buffer[30];
    string months[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    string days[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    time_t rawtime;
    struct tm * ptm;

    // Get time
    time ( &rawtime );
    ptm = gmtime ( &rawtime );
    sprintf(now_buffer,"%s, %02d %s %04d %02d:%02d:%02d ", days[ptm->tm_wday].c_str(), ptm->tm_mday, months[ptm->tm_mon].c_str(), 1900 + ptm->tm_year, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    return string(now_buffer);
} // end now
