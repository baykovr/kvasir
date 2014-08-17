kvasir
======

Simple HTTP scanner using no external libraries.

Use ulimit to increase the number of max open file descriptors if using many sockets.

Note: There is sparse documentation since this is a toy project, if you would like an example of
socket programming see scan.c

Usage
=====

in kvasir.h modify

    #define VERBOSE 0
    #define RAIL_SIZE 16
    const char* ACTIVE_MSG = "SOME HTTP REQ"

There are 16 rails of variable sized, defined by RAIL_SIZE. In this example 16*16 256 active threads will
be created. Typically 4096 file descriptors are allowed by default on my linux systems, you can increase this number using the ulimit command.

    cat /proc/sys/fs/file-max
    # ulimit -Hn
    # ulimit -Sn

    vi /etc/security/limits.conf
    username soft nofile 4096
    username hard nofile 10240


in scan.h modify

    #define DEBUG 0
    #define TIMEOUT 2 /*seconds*/
    #define MAX_RESPONSE_SIZE 4096

Responces are display in standard out. MAX_RESPONSE_SIZE is specified in bytes after which the socket will close.
