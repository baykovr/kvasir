kvasir
======

A fork/exec threaded HTTP scanner using no external libraries.

Use ulimit to increase the number of max open file descriptors if using many sockets.

Note: There is sparse documentation since this is a toy project, if you would like an example of
socket programming see scan.c

Configuration
=====

@kvasir.h

    #define VERBOSE 0
    #define RAIL_SIZE 16
    const char* ACTIVE_MSG = "SOME HTTP REQ"

ACTIVE_MSG is your http request, pretty simple.

RAIL:
Rails are groups of threads, by default there are 16 rails. (See kvasir.c @ int proc_scheduler(...) )
Kvasir will wait for all threads in a rail to complete before starting spawning a new one, so at all times maintaning 16 rails active at once (until done).

RAIL_SIZE:
Each rail has a number of threads which it spawns simultaniously, this thread number is specified by RAIL_SIZE.

For example, with the default RAIL_SIZE of 16 (and 16 RAILS) gives 16*16  = 256 active threads. That's a lot of threads, no more than 256 will be active at once. 

Each thread in the rail connects using standard socket to issue the HTTP request. Recall that a socket is simply a file descriptor (fd), and most linux systems have a limit on the maximum number of fd's.

Normally the maximum is 4096 fd's, meaning you can't have a very big rail size...but you can increase this number using the ulimit command.

    See the current value and set it temporarily
    # cat /proc/sys/fs/file-max
    # ulimit -Hn
    # ulimit -Sn
     
    # Set the value permenantly
    vim /etc/security/limits.conf
    username soft nofile 4096
    username hard nofile 10240

The number of threads is up to you, it's super inefficient compared to mass-scan, netmap or zmap so likely you'll run out of CPU long before you saturate your network card but you'd be surprised at the number of packets you can generate with this simple tool.


Scanner
=====
@scan.h modify

    #define DEBUG 0
    #define TIMEOUT 2 /*seconds*/
    #define MAX_RESPONSE_SIZE 4096

DEBUG will show additional connection information.

TIMEOUT is the maximum time to wait for the end-host connection to establish, AND the maximum time to wait after the connection is established. (How long to wait for connect, and then how long to wait for data to start coming in)

The MAX_RESPONSE_SIZE allows you to specify (in bytes) the upper bound of data to recvieve on the socket, the default is 4KB

Responces are display in standard out just pipe it to a file:

    kavsir [options] >> some_file.txt

For improved performance you should impliment file handleing in the recv section of scan.c (watch out for threading related issues). 

.
