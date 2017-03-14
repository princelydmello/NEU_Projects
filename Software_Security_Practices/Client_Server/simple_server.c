// Simple server that takes and prints the usernames and passwords from the client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>


#define PORT 222	

int main(void) {
	int sockfd, new_sockfd;  
	struct sockaddr_in host_addr, client_addr;	
	socklen_t sin_size;
	int recv_length=1, yes=1;
	char buf, buffer[1024];
	time_t t;
	//FILE *f;	
	
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		printf("Error in socket");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		printf("Error setting socket option SO_REUSEADDR");
	
	host_addr.sin_family = AF_INET;		
	host_addr.sin_port = htons(PORT);	 
	host_addr.sin_addr.s_addr = INADDR_ANY; 
	
	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		printf("Error binding to socket");

	if (listen(sockfd, 5) == -1)
		printf("Error listening on socket");

	while(1) {    
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if(new_sockfd == -1)
			printf("Error accepting connection");
		t = time(NULL);		
		printf("%s",ctime(&t));		
		printf("IP address:%s \t port: %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));	
		send(new_sockfd, "Please enter the Username and Password\n", 40, 0);
		recv_length = recv(new_sockfd, &buffer, sizeof(buffer), 0);
		int i=0;
		while(recv_length > 0) {
			if(i==0)
			{
				printf("Username:");
				i=1;
			}
			else
			{
				printf("Password:");	
				i=0;
			}
			
			int i;
			for(i=0;buffer[i]!='\n'&&i<10;i++)							
			printf("%c",buffer[i]);

			printf("\n");
			recv_length = recv(new_sockfd, &buffer, sizeof(buffer), 0);
			
		}
		close(new_sockfd);
	}
	return 0;
}
