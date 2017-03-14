// Simple client to open a file and send the usernames and passwords over to the server
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
  int clientSocket;
  char buffer[1024]; 	
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  FILE * fp,*fp1;
  char * line = NULL,* line1 = NULL;
  size_t len = 0,len1 = 0;
  ssize_t read,read1;	
 
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(222);
  serverAddr.sin_addr.s_addr = inet_addr("192.168.56.102");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
  
  recv(clientSocket, buffer, 1024, 0);
	
   fp = fopen("user.txt", "r");
   if (fp == NULL)
       printf("Error opening file");

  while ((read = getline(&line, &len, fp)) != -1) {
        fp1 = fopen("pwd.txt", "r");
        if (fp1 == NULL)
	       printf("Error opening file");  
    	while ((read = getline(&line1, &len1, fp1)) != -1) {
	 
	 printf("%s", line);
	 printf("%s", line1);     
	 send(clientSocket, line , read , 0);
	 send(clientSocket, line1 , read1 , 0);
	}

    }
	
  return 0;
}
