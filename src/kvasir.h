#include <stdio.h>
#include <stdlib.h>

#include <limits.h>

#define VERBOSE 0
#define RAIL_SIZE 16

int task_rails(char* rails[], int num_rails);
int dispence_rail(char* rail[],int rail_size,const char* payload);
int proc_scheduler(char* host_file_path);

const char* ACTIVE_MSG = "GET index.html HTTP1.0\r\n\r\n";
