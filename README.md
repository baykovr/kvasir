kvasir
======

Simple HTTP scanner using no external libraries.

Use ulimit to increase the number of max open file descriptors if using many sockets.

Usage
=====

in kvasir.h modify

    #define VERBOSE 0
    #define RAIL_SIZE 16
    const char* ACTIVE_MSG = "SOME HTTP REQ"
    
in scan.h modify

    #define DEBUG 0
    #define TIMEOUT 2 /*seconds*/
    #define MAX_RESPONSE_SIZE 4096
