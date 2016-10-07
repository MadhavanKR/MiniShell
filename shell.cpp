#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>

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
		   printf("k=%d and position=%d\n",k,position);
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
   arg[k][position++]='\0';
   printf("k=%d and position=%d\n",k,position);
   *count=k;
   return arg;

}

int shell(char ***arg,int count)
{
	int fd[2],val,k=0,pid;
	
	while(k<=count)
	{
		if((pid=fork())==-1)
		{
			perror("vfork error");
			exit(0);
		}
		else if(pid==0)
		{
			if(k%2)
			{
				//reader process
				//k++;
				
				close(fd[1]); //close the write end
				close(0);
				int r=dup2(fd[0],0);
				if(r==-1)
				{
					perror("dup2 error");
					exit(0);
				}
				if(execvp(arg[k][0],arg[k])==-1)
				{
					perror("exec error");
					exit(0);
				}
			}
			else
			{
				//writer process
				//k++;
				close(fd[0]); //close the read end
				close(1);
				int r=dup2(fd[1],1);
				if(r==-1)
				{
					perror("dup2 error");
					exit(0);
				}
				if(execvp(arg[k][0],arg[k])==-1)
				{
					perror("exec error");
					exit(0);
				}
			}
		}
		else
		{
			int status;
			waitpid(pid,&status,0);
			//printf("output is here!\n");
			char buff[101];
			int b;
			close(fd[1]);
			if(k==count)
			{
				struct stat var;
				close(fd[1]);
				int ret=fstat(fd[0],&var);
				char *buff=new char[var.st_size+1];
				int r=read(fd[0],buff,var.st_size);
				if(r==-1)
				{
					perror("read error");
					exit(0);
				}
				buff[r]='\0';
				printf("%s\n",buff);
				return 0;
			}
		}
		k++;
	}
	
}

int main()
{
	char *cmd,***arg;
	int count;
	while(1)
	{
		printf(">");
		cmd=read();
		if(strcmp(cmd,"exit")==0)
		{
			printf("terminating shell! ciao!\n");
			exit(0);
		}
		else
		{
			arg=parse(cmd,&count);
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
			int status=shell(arg,count);
			printf("status is as follows %d\n",status);
			free(cmd);
			free(arg);
			printf("one command done! next please!\n");
		}
	}
}
