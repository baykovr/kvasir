#include "kvasir.h"
#include "scan.h"


// int proc_multi_rails(char** rails[])
// {
// 	unsigned int num_children = MAX_RAILS;

// 	pid_t pids[num_children];
	
// 	unsigned int i;
// 	for(i=0;i<num_children;i++)
// 	{
// 		if ((pids[i] = fork()) < 0)
// 		{
//     		perror("fork");
//     		abort();
//   		}
//   		else if (pids[i] == 0)
//   		{
//   			/*Process a single rail from rails*/
//   			unsigned int r_item_c;
// 			for(r_item_c=0;r_item_c<RAIL_SIZE;r_item_c++)
// 	    	{printf(" [+] %s\n",rails[i][r_item_c]);}
// 	  		printf("-*-\n");

//     		proc_rail(rails[i]);
//     		exit(0);
//   		}
// 	}
// 	/*wait for the children*/
// 	int status;
// 	pid_t pid;
// 	while(num_children>0)
// 	{
// 		pid = wait(&status);
// 		if(DEBUG)
// 		{
// 			printf("child %ld finished: %d\n",(long)pid,status);
// 		}	
// 		num_children--;
// 	}
// 	return 0;
// }

int proc_rail(char* rail[])
{
	unsigned int num_children = RAIL_SIZE;

	pid_t pids[num_children];
	
	unsigned int i;
	for(i=0;i<num_children;i++)
	{
		if( rail[i] == NULL)
		{
			/*no more tasks in this rail*/
			if(VERBOSE){printf("[EXIT] %s\n",rail[i]);}
			break;
		}
		else
		{
			if ((pids[i] = fork()) < 0)
			{
    			perror("fork");abort();
  			}
  			else if (pids[i] == 0)
  			{
  				/*Process one item per child*/
  				scan(rail[i], ACTIVE_MSG);
  				free(rail[i]);
  				exit(0);
  			}

		}
	}
	/*wait for the children to exit*/
	int status;
	pid_t pid;
	while(num_children>0)
	{
		pid = wait(&status);
		if(VERBOSE)
		{
			printf("child %ld finished: %d\n",(long)pid,status);
		}	
		num_children--;
	}
	return 0;
}

int proc_scheduler(char* host_file_path)
{
	if(VERBOSE)
		{printf("Reading %s\n",host_file_path);}
	
	FILE* hosts;
  	char* read_line = NULL;
  	int eof         = 0;
  	size_t  len     = 16;
  	hosts = fopen(host_file_path,"r");
  	if(hosts==NULL) 
    	{perror("Cannot open file\n");exit(-1);}

    char** rails[MAX_RAILS];
    while(!eof)
    {
		/*construct rail*/
    	char* rail[RAIL_SIZE];
    	
    	unsigned int r_item_c;
    	for(r_item_c=0;r_item_c<RAIL_SIZE;r_item_c++)
    	{
    		char *line = malloc(16);
			ssize_t line_len = getline(&line,&len,hosts);
			/*strip the newline character*/
    		line[strlen(line) - 1] = '\0';

    		/*check for eof*/
    		if(line_len < 8 )
    		{
      		if(VERBOSE)
      		  {printf("[READER] Reached EOF or line len < 8\n");}
      			/*add end rail delim*/
      			rail[r_item_c] = NULL;
      			eof = 1;
      			break;
    		}
    		else
    		{
    			rail[r_item_c] = line;
    		}
    	}
	    proc_rail(rail);
	    if(eof)
	    	{break;}
    }
    return 0;
	//int fd = read("/proc/sys/kernel/pid_max");
	//cat /proc/sys/kernel/pid_max
	// time 5 hours is 18000s
	// thats 6000 iterations at 3s timeout
	// [12833,6000]
	// 77M lines
	// in 4 pieces, 20,000,000
	// 
	// 77000000 * 16 ~ 1.1GB
	// [32,601562.5]
	// X = 32 
	// 77000000 = X*Y
	// X:Y [32,2406250]
	// X:Y [128,601562.5]
	// X jobs per swarm
	// Y swarms (max TIMEOUT (3s) execution)
	//construct Y work arrays of size X
	//buid array from lines
	//fire swarms each with 1 work array of size X
}

int main(int argc, char* argv[])
{
	if(argc!=2)
	{
		printf("usage kvasir host_file\n");
		return 0;
	}

	proc_scheduler( argv[1] );

	return 0;
}