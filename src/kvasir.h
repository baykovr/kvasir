#include <stdio.h>
#include <stdlib.h>

#include <limits.h>

#define VERBOSE 1
#define MAX_RAILS 2
#define RAIL_SIZE 8


int task_rails(char* rails[], int num_rails);
int dispence_rail(char* rail[],int rail_size,const char* payload);
int proc_scheduler(char* host_file_path);

const char* ACTIVE_MSG = "GET /fish.jpg?&t=4426 HTTP/1.1\nUser-Agent: Update\nHost: www.dianwofacai.com\nCache-Control: no-cache\r\n\r\n";
const char* msg_2 = "GET /29af9cdc/page_12082223.html HTTP/1.1\nAccept: */*\nCookie: XX=0; BX=0User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Win32)\nHost: gorush.dyndns-web.com\nConnection: Keep-Alive\nCache-Control: no-cache\nPragma: no-cache;\r\n\r\n";
const char* msg_3 = "GET /images/logos.gif?1f5428=8212640 HTTP/1.1\nUser-Agent: Opera/9.50 (Windows NT 6.0; U; en)\nHost: boyabateml.k12.tr\nCache-Control: no-cache\r\n\r\n";