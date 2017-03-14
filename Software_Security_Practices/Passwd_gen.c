// Program to generate all combinations of 62 characters of length 16 and generating a hash for them.   

#include <stdio.h>
#include <openssl/md5.h>
#include <sys/resource.h>

void print_combinations(int a[16],char seed[62],FILE *f)
{
	//code to remove repeating passwords
	int i,x=0,y=0;
	for(i=0;i<16&&x!=1;i++)
	{
		if(a[i]==62)
		x=1;		
	}	
	if(x!=1)
	{
		for (i=0;i<16&&y!=1;i++)
		{	
			int j;
			for(j=i+1;j<16&&y!=1;j++)
			{
				if(a[i]!=a[j])
				y=1;			
			}
		}
	}	
	
	if(x==0&&y==1)	
	{	
		char pass[16];		
		for(i=0;i<16;i++)
		{
			pass[i] = seed[a[i]];			
			fprintf(f,"%c",seed[a[i]]);
			if(i==15)
			fprintf(f,"\t");
		}
		
		//Generate hash values
		unsigned char digest[100];
		MD5_CTX context;
		MD5_Init(&context);
		MD5_Update(&context, pass, sizeof(pass));
		MD5_Final(digest, &context);
		for(i = 0; i < MD5_DIGEST_LENGTH; i++) fprintf(f,"%02x", digest[i]);
		fprintf(f,"\n");
	}
		
	//code to generate passwords
		
	for(i=14;i>=0;i--)	
	{	
		if(a[i]<62 && a[i+1]<62)
		{
		a[i+1]=a[i+1]+1;
		print_combinations(a,seed,f);	
		}	
		else if(a[i+1]==62 && a[i]<62)
		{
			a[i]=a[i]+1;
			a[i+1]=0;
			print_combinations(a,seed,f);
		} 	
	}
}

int main()
{
	char seed[] ={'A','B','C','D','E','F','G','H','J','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','1','2','3','4','5','6','7','8','9','0'};
	
	int i,a[16];

	for(i=0;i<16;i++)
	a[i]=0;
 	
	FILE *f;
	if((f=fopen("pwd.txt","w"))==NULL) 
	printf("\nCannot open pwd.txt for writing\n");

	//code to increase stack limit	
	const rlim_t kStackSize = 1000 * 1024 * 1024;   // min stack size = 16 MB
    	struct rlimit rl;
    	int result;

    	result = getrlimit(RLIMIT_STACK, &rl);
    	if (result == 0)
    	{
        	if (rl.rlim_cur < kStackSize)
       	 	{
        		rl.rlim_cur = kStackSize;
       	    		result = setrlimit(RLIMIT_STACK, &rl);
       	     		if (result != 0)
       	     		fprintf(stderr, "setrlimit returned result = %d\n", result);
       	 	}
    	}

	
	print_combinations(a,seed,f); 

	fclose(f);
	return 0;
}
