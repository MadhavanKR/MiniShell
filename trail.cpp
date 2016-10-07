#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>

#define buffsize 1024
#define delim " \t\r\n\a"
using namespace std;

/* this function will read character by character and store it into the
  string and return the string when ENTER key is pressed*/
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

/* this function will parse the entire strings into various commands
 * that are part of the string and each command will be saperated into
 * command and it's arguments
 * ex : a string ls -l || wc -c will be parsed into
 * 1) ls -l and
 * 2) wc -c
 * */
	
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
	//printf("memory allocation done \n");
   token=strtok(cmd,delim);
   while(token!=NULL)
   {
	   //printf("token = %s\n",token);
	   if(strcmp(token,"||") == 0)
	   {
		//printf("token is ||\n");
		   arg[k][position++]='\0';
		   position=0;
		   k++;
	   }
	   else
	   {
		   arg[k][position]=token;
		   position++;
		   //cout<<"position = "<<position<<endl;
	   }
	   token=strtok(NULL,delim);
   }
   arg[k][position]='\0';
   *count=k;
   return arg;
}

/* this is the core of the program. Execute function will fork exec every
 * command and interprocess communication between two commands is facilitated
 * using pipes
 * -> first command reads input from stdin and writes to pipe num 0
 * -> second command reads input from pipe num 0 and writes to pipe num 1
 * -> third command reads input from pipe num 1 and writes to pipe num 2
 * -> nth command reads input from pipe num n-2 and writes to pipe num n-1
 * -> last command reads input from pipe num n-1 and writes to stdout
 * */


int execute(char*** arg,int count)
{
	int fd[100][2],k=0,pid;
	count++;
	//printf("count = %d\n",++count);
	for(int l=0;l<=count;l++)
	   pipe(fd[l]);
	while(k<count)
	{
		//cout<<"how many times am i coming in K loop?"<<endl;
		//printf("k=%d\n",k);
		if((pid=fork())==-1)
		{
			perror("vfork error");
			exit(0);
		}
		else if(pid==0)
		{
			if(count == 1)
			{
			//cout<<"in the single command block"<<endl;
				if(execvp(arg[k][0],arg[k])==-1)
				{
					perror("exec error");
			//cout<<"something went wrong!"<<endl;
					exit(0);
				}
				exit(0);
			}
			else if(k%2==0 && k==0)
			{
			//first command, reads from stdin and writes to pipe number k	
				close(1);
				int r=dup2(fd[k][1],1);
				if(r==-1)
				{
					perror("dup2 error");
					exit(0);
				}
			//closing all the unwanted file descriptors
				for(int l=0;l<=count;l++)
				{
					close(fd[l][0]);
					close(fd[l][1]);
				}
				cout<<"executing the "<<k<<" th command"<<endl;
				if(execvp(arg[k][0],arg[k])==-1)
				{
					perror("exec error");
					exit(0);
				}
				exit(0);
			}
			else
			{
				//writer process
				//k++;
				if(k!=count-1)
				{
					close(fd[k][0]); //close the read end
					close(1);
					int r=dup2(fd[k][1],1);
					if(r==-1)
					{
						perror("dup2 error");
						exit(0);
					}
				}
				
				close(0);
				int w=dup2(fd[k-1][0],0);
				if(w==-1)
				{
					perror("dup2 error");
					exit(0);
				}

				for(int l=0;l<=count;l++)
				{
					close(fd[l][0]);
					close(fd[l][1]);
				}
				
				if(execvp(arg[k][0],arg[k])==-1)
				{
					perror("exec error");
					exit(0);
				}
				exit(0);
			}
			
		}
		k++;
	//cout<<"k incremented!"<<endl;
	}
	
	for(int l=0;l<count+1;l++)
	{
		close(fd[l][0]);
		close(fd[l][1]);
	}
	int status,wpid;
	do
	{
		//cout<<"how many times am i coming here?"<<endl;
		wpid=waitpid(pid,&status,WUNTRACED);
	}while(!WIFEXITED(status)&&!WIFSIGNALED(status));
	return 1;
}

void print(char ***arg,int count)
{
	int i=0;
	while(i<=count)
	{
		for(int j=0;arg[i][j]!=NULL;j++)
		    cout<<j<<": "<<arg[i][j]<<" ";
		i++;
		cout<<endl;
	}
}

/*main is the driver program*/

int main()
{
	char *cmd,***arg;
	int count,k;
	while(1)
	{
	  printf(">:");
	  cmd=read();
	//printf("cmd = %s\n",cmd);
	  if(strcmp(cmd,"exit")==0)
	     exit(0);
	  arg=parse(cmd,&count);
	//print(arg,count);
	  execute(arg,count);
	//printf("i returned from the function\n");
	}
}
