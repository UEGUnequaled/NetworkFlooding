#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <time.h>

//If system calls goes wrong, we terminate client
void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    
    if (argc < 3) 
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) 
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR with connecting");
    }
   	//infinite loop until we exit (will implement later)
    while(1)
    {
    	//display localtime and current user
    	printf("%s: "getenv("USER"));
    	bzero(buffer,256);
    	fgets(buffer,255,stdin);
    	n = write(sockfd,buffer,strlen(buffer));
    	if (n < 0)
   	 	{ 
   	 		error("ERROR writing to socket");
   		}
	    bzero(buffer,256);
	    n = read(sockfd,buffer,300);
	   	if (n < 0)
    	{	
        	 error("ERROR reading from socket");
    	}
    	//get local time
    	time_t currtime = time(NULL);
    	//display buffer from server with timestamp
    	printf("%s:%s\n",asctime(localtime(&currtime)), buffer);
    }
    /*we should never exit here unless we input an exit func*/
    return 0;
}
