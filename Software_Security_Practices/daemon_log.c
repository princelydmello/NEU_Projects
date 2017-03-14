// C Program to create a daemon and sending signals to pause or kill the process and logging the actions.
#include<unistd.h>
#include<signal.h>
#include<syslog.h>
#include<errno.h>
#include<stdio.h>
#include<time.h>

#define LOG_FILE "logfile.txt"

//static volatile int exit_flag=0;

//static FILE *f;

void log_message(char *f,char *m)
{
	char time1[20];	
	FILE *logfile;
	logfile=fopen(f,"a");
	if(!logfile) return;

	time_t clock = time(NULL);
	
	fprintf(logfile,"%s->%s\n",ctime(&clock),m); 
	fclose(logfile);
}

void signalhandlr(int signal)
{
	
	switch(signal)
	{
		case SIGINT:
		log_message(LOG_FILE,"Received SIGINT signal");
		exit(0);		
		break;
		
		case SIGQUIT:
		log_message(LOG_FILE,"Received SIGQUIT signal");
		exit(0);		
		break;

		case SIGTRAP:
		log_message(LOG_FILE,"Received SIGTRAP signal");
		break;
	
		case SIGABRT:
		log_message(LOG_FILE,"Received SIGABRT signal");
		exit(0);		
		break;

		case SIGKILL:
		log_message(LOG_FILE,"Received SIGKILL signal");
		exit(0);
		break;

		case SIGTERM:
		log_message(LOG_FILE,"Received SIGTERM signal");
		exit(0);			
		break;

		case SIGCONT:
		log_message(LOG_FILE,"Received SIGCONT signal");
		break;

		case SIGTSTP:
		log_message(LOG_FILE,"Received SIGTSTP signal");
		exit(0);	
		break;

		default:
		log_message(LOG_FILE,"Received unhandled signal");
	} 
}
int main(){
	
	struct sigaction newSigAction;
	int pid=daemon(1,1);
	//printf("%i\n",pid);
	
	if(pid==0)
	{
		printf("Daemon created successfully with id %i\n",getpid());	
	}
	else
	{
		printf("Daemon creation failed\n");
	}
	while(0==0)
	{	
		newSigAction.sa_handler=&signalhandlr;
		sigaction(SIGINT,&newSigAction,NULL);
		sigaction(SIGQUIT,&newSigAction,NULL);
		sigaction(SIGTRAP,&newSigAction,NULL);
		sigaction(SIGABRT,&newSigAction,NULL);
		sigaction(SIGTERM,&newSigAction,NULL);
		sigaction(SIGKILL,&newSigAction,NULL);
		sigaction(SIGTERM,&newSigAction,NULL);
		sigaction(SIGCONT,&newSigAction,NULL);
		sigaction(SIGTSTP,&newSigAction,NULL);
	}	
	return 0;
}

