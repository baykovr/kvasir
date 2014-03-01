#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define DEBUG   0
#define TIMEOUT 2 /*seconds*/

#define MAX_RESPONSE_SIZE 4096

int scan(char* host, const char* payload);