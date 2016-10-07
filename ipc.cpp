#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<errno.h>
#include<stdlib.h>

int main()
{
	char cmd1[1024],cmd2[1024];
	int fd[2],val,k=0,pid;
	printf("enter the command to left of pipe\n");
	scanf("%s",cmd1);
	printf("enter the command to right of pipe\n");
	scanf("%s",cmd2);
	val=pipe(fd);
	if(val==-1)
	{
		perror("pipe error");
		exit(0);
	}
	while(k<2)
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
				if(execlp(cmd2,cmd2,NULL)==-1)
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
				if(execlp(cmd1,cmd1,NULL)==-1)
				{
					perror("exec error");
					exit(0);
				}
			}
		}
		else
		{
			wait(NULL);
			//printf("output is here!\n");
			char buff[101];
			int b;
			close(fd[1]);
			if(k==2)
			{
				while((b=read(fd[0],buff,100))!=-1)
				{
					buff[b]='\0';
					printf("%s",buff);
				}
			}
		}
		k++;
	}
	return 0;
}
