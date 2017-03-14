// Program to check if a username is alphanumeric and password is alphanumeric or contains small set of special characters
#include <stdio.h>
#include <string.h>

int main(){
	char uname[20],pwd[20],cmd[100];
	
	printf("Enter the username and password not greater than 20 characters each \n");
	printf("Username:");
	scanf("%s",&uname);
	printf("Password:");
	scanf("%s",&pwd);
	
	if(strlen(uname)<=20&&strlen(pwd)<=20)
	{
		int i,j;
		
		char mod_uname[40],mod_pwd[40];
		//adding escape sequence for special characters
		for(i=0,j=0;i<=strlen(uname);i++,j++)
		{	
			if(uname[i]=='$'||(uname[i]=='-'&&i==0)||uname[i]=='%'||uname[i]=='\''||uname[i]=='\"')
			{	
				if(uname[i]=='%')
				{
					mod_uname[j]= '%';				
					j++;
					mod_uname[j]= '%';				
					j++;
					mod_uname[j]= '%';				
				}	
				else
				mod_uname[j]= '\\';

				j++;		
				mod_uname[j]=uname[i];
			}
			else if(i != strlen(uname))
			mod_uname[j] = uname[i];
			else		  				
      mod_uname[j]='\0';					
		}

		for(i=0,j=0;i<=strlen(pwd);i++,j++)
		{		
			if(pwd[i]=='$'|| ( pwd[i]=='-'&& i==0)||pwd[i]=='%'||pwd[i]=='\''||pwd[i]=='\"')
			{	
				if(pwd[i]=='%')
				{
					mod_pwd[j]= '%';
					j++;
					mod_pwd[j]= '%';
					j++;
					mod_pwd[j]= '%';
				}
				else
				mod_pwd[j]= '\\';

				j++;		
				mod_pwd[j]=pwd[i];
			}
			else if(i != strlen(pwd))
			mod_pwd[j] = pwd[i];
			else		  				
			mod_pwd[j]='\0';				
		}
		
		
		
		sprintf(cmd,"printf \"%s\n%s\" | perl regex.pl",mod_uname,mod_pwd);
		//printf(cmd);
		int x = system(cmd);
		//printf("%s",x);		

		if(x!=0)
		printf("Please enter a Username that is alphanumeric and a Password that contains numbers, alphabets or special caracters such as @,#,$,%,^,&,* \n");			
		else
		printf("The Username and Password are valid\n");		
			
		
	}
	else
	{	
		if(strlen(uname)>20)
		printf("The entered Username should be less than 20 characters\n");
		if(strlen(pwd)>20)
		printf("The entered Password should be less than 20 characters\n");	
	}	
	return 0;
} 
