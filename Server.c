#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hacking.h"
#include <unistd.h>


#define PORT 1337

char buffer1[128];

int copying (int l)
	{
	int i;
	unsigned int j=1;
	char buffer2[8];
	
	printf("No Canary Found, %d\n",l);
	
	for(i=0; i<l; i++)
		{	
		buffer2[i]=buffer1[i];
		}	
	
	return 0;
	}			


int main()
	{
		
	int s_server, s_client;
	struct sockaddr_in s_addr, c_addr;
	socklen_t size;
	int len_r=1;
	int yes=1;
	int x,l, i;
	char a;
	
	
	pid_t pid;
	
	
	if((s_server=socket(PF_INET, SOCK_STREAM, 0))== -1)
		fatal("In Socket");
		
	
	if (setsockopt(s_server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		fatal("In Socket Options");
	
	
		
	s_addr.sin_family = AF_INET;		 
	s_addr.sin_port = htons(PORT);	 
	s_addr.sin_addr.s_addr = INADDR_ANY; 
	memset(&(s_addr.sin_zero), '\0', 8); 
		
	
	if(bind(s_server, (struct sockaddr *)&s_addr, sizeof (struct sockaddr))==-1)
		fatal("In Bind");
		
	
	if (listen(s_server, 5) == -1)
		fatal("In Listen");
		
	bzero(buffer1,128);
	
	while(1) 
		{    
		size = sizeof(struct sockaddr_in);
		s_client = accept(s_server, (struct sockaddr *)&c_addr, &size);
		if(s_client == -1)
			fatal("In Accept");
			
		pid = fork();
			
		if (pid < 0) 
			{
			perror("ERROR on fork");
			exit(1);
			}	
		if(pid==0) 
			{
			close(s_server);	
			
			len_r = recv(s_client, buffer1, 60, 0);
			dump(buffer1,len_r);
			
			x=copying(len_r);
			printf("Canary Found\n");
			
			send(s_client, "found", 5,0);
			
			break;
			}	 	
		}
	return 0;
	}	
