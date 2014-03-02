#include "kvasir.h"
#include "scan.h"

<<<<<<< HEAD

=======
>>>>>>> 7f27ec7092c8ba2b255691326f8c5887cc19d7f9
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
  				free(rail[i]); /*the most important line*/
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