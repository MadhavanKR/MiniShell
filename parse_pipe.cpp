#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>

#define buffsize 1024
#define delim " \t\r\n\a"
using namespace std;

char* read()
{
	char ch,*buff=new char[buffsize];
	int position=0;
	while((ch=getchar())!='\n')
	{
		buff[position++]=ch;
	}
	buff[position]='\0';
	return buff;
}

char*** parse(char *cmd,int *count)
{
   int position=0;
   int k=0;
   char ***arg=new char**[16],*token;
   for(int i=0;i<15;i++)
   {
	   arg[i]=new char*[16];
	   for(int j=0;j<15;j++)
	      arg[i][j]=new char[1024];
   }
   printf("memory allocation done \n");
   token=strtok(cmd,delim);
   while(token!=NULL)
   {
	  // printf("token = %s\n",token);
	   if(strcmp(token,"||") == 0)
	   {
//		   printf("token is ||\n");
		   arg[k][position++]='\0';
		   position=0;
		   k++;
	   }
	   else
	   {
		   arg[k][position]=token;
		   position++;
	   }
	   token=strtok(NULL,delim);
   }
   *count=k;
   return arg;
}

int main()
{
	char *cmd,***arg;
	int count,k;
	printf("please enter the command to parse\n");
	cmd=read();
	arg=parse(cmd,&count);
	//printf("returned from the function \n");
	for(int i=0;i<=count;i++)
	{
		int j=0;
		while(arg[i][j]!='\0')
		{
			//printf("can i print this \n");
		    printf("%s\n",arg[i][j]);
		    j++;
		}
	}
	return 0;
}
