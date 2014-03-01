#include "scan.h"


int scan(char* host, const char* payload)
{
	//printf("%s\n",host);
	int sockfd,res,valopt;
  	long arg;
 	fd_set sock_set;
  	socklen_t lon;
  	struct sockaddr_in addr;
	struct addrinfo hints, *servinfo;
	struct timeval timeout;
  	
	/*fill timeout*/
  	timeout.tv_sec  = TIMEOUT;
  	timeout.tv_usec = 0;

  	/*fill target sockaddr_in*/
	memset(&hints, 0, sizeof(hints));

	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags    = AI_PASSIVE;

	if( getaddrinfo(host,"80",&hints,&servinfo) !=0 )
	{
		if(DEBUG)
			{fprintf(stderr, "[scan] (%s)\n", strerror(errno));}
		return -1;
	}

	/*register socket*/
	sockfd = socket(servinfo->ai_family,servinfo->ai_socktype,servinfo->ai_protocol);
	if(sockfd < 0)
	{
		if(DEBUG)
			{fprintf(stderr, "[scan] (%s)\n", strerror(errno));}
		return -1;
	}

	/*get socket mode*/
	arg = fcntl(sockfd, F_GETFL, NULL);
	if(arg < 0) 
    { 
		if(DEBUG)
      	{fprintf(stderr, "[scan] (%s)\n", strerror(errno)); }
      	if(sockfd>0)
        	{close(sockfd);}
        return -1;
    }
    /*non-blocking*/
    arg |= O_NONBLOCK; 

    /*set socket mode*/
    if( fcntl(sockfd, F_SETFL, arg) < 0) 
    {
      if(DEBUG)
      	{fprintf(stderr, "[scan] (%s)\n", strerror(errno));}
      if(sockfd>0)
		{close(sockfd);}
      return -1;
    }

    /*connect*/
	res = connect(sockfd, servinfo->ai_addr,servinfo->ai_addrlen);

    if (errno == EINPROGRESS) 
    { 
		while(1)
		{
			FD_ZERO(&sock_set); 
			FD_SET(sockfd, &sock_set); 
			res = select(sockfd+1, NULL, &sock_set, NULL, &timeout); 

           /*errno set to something other than in progress*/
			if (res < 0 && errno != EINTR) 
			{ 
            if(DEBUG)
              {fprintf(stderr, "[scan] connect %s\n", strerror(errno));}
            if(sockfd>0)
              {close(sockfd);}
          	return -1;
           }
           else if (res > 0) 
           { 
              /*socket select for write*/
              lon = sizeof(int); 
              if (getsockopt(sockfd,SOL_SOCKET,SO_ERROR,(void*)(&valopt),&lon)< 0) 
              { 
                if(DEBUG)
                    {fprintf(stderr, "[scan] getsockopt %s\n",strerror(errno));}
                if(sockfd>0)
                    {close(sockfd);}
                return -1;
              } 
              /*check return value*/ 
              if (valopt) 
              { 
                  if(DEBUG)
                    {fprintf(stderr, "[scan] delayed con %s\n", strerror(valopt));}
                  if(sockfd>0)
                    {close(sockfd);}
                return -1;
              }
              /*socket is ready*/
              break; 
           } 
           /*timed out*/
           else 
           { 
				if(DEBUG)
				  {fprintf(stderr, "[scan] con timed out\n",strerror(errno));}
				if(sockfd>0)
				  {close(sockfd);}
				return -1;
           } 
        }
     } 
     /*some error with connect*/
     else 
     { 
		if(DEBUG)
		  {fprintf(stderr, "[scan] con %s\n", errno, strerror(errno)); }
        if(sockfd>0)
          {close(sockfd);}
		return -1;
     } 

     /*set send timeout*/
	if (setsockopt (sockfd, SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout))<0)
	{
        if(DEBUG)
          {fprintf(stderr, "[scan] send timo\n",strerror(errno));}
        if(sockfd>0)
          {close(sockfd);}
        return -1;
	}
	/*send*/

	if( send(sockfd, payload, strlen(payload), 0) < 0)
	{
		if(DEBUG)
		  {fprintf(stderr, "[scan] send \n",strerror(errno));}
		return -1;
	}

	/*set recv timeout*/
	if (setsockopt (sockfd, SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout))<0)
	{
        if(DEBUG)
          {fprintf(stderr, "[scan] send timo\n",strerror(errno));}
        if(sockfd>0)
          {close(sockfd);}
        return -1;
	}

	/*select on socket, wait for data w/ timeout*/
    FD_ZERO(&sock_set);
    FD_SET(sockfd, &sock_set);
    int n = sockfd + 1 ;
    int rv = select(n, &sock_set, NULL, NULL, &timeout);
    if (rv == -1) 
    {
       if(DEBUG)
          {fprintf(stderr, "[scan] send timo\n",strerror(errno));}
       if(sockfd>0)
        {close(sockfd);}
      	return -1;
    }
    else if (rv == 0)
    {
        if(DEBUG)
          {fprintf(stderr, "[scan] timed out.\n",strerror(errno));}
        if(sockfd>0)
          {close(sockfd);}
      	return -1;
    }
    /*recieve*/
    else
    {
    	/*recv data*/
    	int bytes_recv;
    	char recv_buffer[128];
    	unsigned int total_bytes_in = 0;
    	while( (bytes_recv = recv(sockfd,recv_buffer,sizeof(recv_buffer)-1,0))>0 )
    	{
    		total_bytes_in += bytes_recv;
    		if(total_bytes_in > MAX_RESPONSE_SIZE ) {break;}
    	}
    	if(sockfd)
    		{close(sockfd);}
    }


	return 0;
}