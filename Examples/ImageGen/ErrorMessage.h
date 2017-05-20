#ifndef G_ERRORMESSAGE_H
#define G_ERRORMESSAGE_H

/*
Generated on: 2011-10-21-15.31.49

Version: 2.5
*/

/*
Notes:
Includes std::pair support
Cross-compatible version
Includes 'delay function' command
Includes optional defining of error functions (allowing them to be replaced with empty placeholders to save compile/run times).


*/

/*
Suggestions for improvements:

Future versions should offer a simplified backtrace macro
That consists of 'boolean value', 'return value'
It should contain line number, file name, boolean value error
*/

#include <stdio.h>
#include <unistd.h>

#ifndef DISPLAY_ERRORS
#define DISPLAY_ERRORS true
#endif

//#ifdef DISPLAY_FUNCTION
//#undef DISPLAY_FUNCTION
//#define DISPLAY_FUNCTION true
//#endif

#ifndef DISPLAY_FUNCTION
#define DISPLAY_FUNCTION false
#endif

#ifndef DELAY_FUNCTION
#define DELAY_FUNCTION false
#endif

#ifndef DELAY_AMOUNT
#define DELAY_AMOUNT 10
#endif

//This Windows/Linux part should be contained in a separate header

#ifdef _WIN64
#define IS_WINDOWS
#elif _WIN32
#define IS_WINDOWS
#endif

#ifdef __APPLE__
#define IS_APPLE
#endif

#ifdef __linux
#define IS_LINUX
#elif _unix
#define IS_LINUX
#endif

#ifdef _posix
#define IS_POSIX
#endif

#define TOTEXT(LINE) #LINE
#define TONAME(LINE) TOTEXT(LINE)
#define ERRORLINE  TONAME(__LINE__) ": " __FILE__

#define ERRORMESSAGE(FUNCTION,OPERATION) \
printf( ERRORLINE ": " #FUNCTION ": " #OPERATION " is true!\n\n");

#define TRACE printf(ERRORLINE " TRACE\n");

#ifndef ERRORFUNCTION
#define ERRORFUNCTION(FUNCTION,OPERATION,RETURN_TYPE)\
if(DISPLAY_FUNCTION)\
{\
printf(TONAME(__LINE__)": " #FUNCTION ": " #OPERATION "\n");\
}\
if(OPERATION)\
{\
    if(DISPLAY_ERRORS)\
    {\
        ERRORMESSAGE(FUNCTION,OPERATION)\
        if(DELAY_FUNCTION)\
        {\
            sleep(DELAY_AMOUNT);\
        }\
    }\
    RETURN_TYPE\
};
#endif

#define ERRORMESSAGECLASS(CLASS,FUNCTION,OPERATION) \
printf( ERRORLINE ": " #CLASS ": " #FUNCTION ": " #OPERATION " is true!\n");

#ifndef ERRORCLASSFUNCTION
#define ERRORCLASSFUNCTION(CLASS,FUNCTION,OPERATION,RETURN_TYPE)\
if(DISPLAY_FUNCTION)\
{\
    printf(TONAME(__LINE__) ": " #CLASS ": " #FUNCTION ": " #OPERATION "\n");\
    if(DELAY_FUNCTION)\
    {\
        sleep(DELAY_AMOUNT);\
    }\
}\
if(OPERATION)\
{\
    if(DISPLAY_ERRORS)\
    {\
        ERRORMESSAGECLASS(CLASS,FUNCTION,OPERATION)\
        if(DELAY_FUNCTION)\
        {\
            sleep(DELAY_AMOUNT);\
        }\
    }\
    RETURN_TYPE\
};
#endif

#define RETURN_VOID return;
#define RETURN_THIS errno = EIO; return *this;
#define RETURN_BOOL return false;
#define RETURN_NULL return NULL;

#define DEBUGINT(X) printf(#X " is: %d\n",X);
#define DEBUGUINT(X) printf(#X " is: %u\n",X);
#define DEBUGFLOAT(X) printf(#X " is: %f\n",X);
#define DEBUGCHAR(X) printf(#X " is: %d %c\n",X,X);
#define DEBUGARRAY(X) printf(#X " is: %s\n",X);
#define DEBUG(X) printf(#X " is: %x\n",X);
#define DEBUGLONG(X) printf(#X " is: %l\n",X);
#define DEBUGULONG(X) printf(#X " is: %lu\n",X);
#define SIZE_TYPE unsigned long

#define SP_RETURN(NUMBER,CLASS,VALUE) return (std::pair< uint8_t, CLASS >( NUMBER, CLASS( VALUE ))); 
#define SP_C(CLASS) std::pair< uint8_t , CLASS >

#endif
