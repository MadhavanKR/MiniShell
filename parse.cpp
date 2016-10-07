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

char** parse(char* cmd)
{
	char **tokens=new char*[buffsize],*token;
	int position=0;
	token=strtok(cmd,delim);
	while(token!=NULL)
	{
		tokens[position++]=token;
		token=strtok(NULL,delim);
	}
	tokens[position]=NULL;
	return tokens;
}

int main()
{
	char *cmd,**arg;
	int i;
	cout<<"enter the string "<<endl;
	cmd=read();
	cout<<"read string is : "<<cmd<<endl;
	arg=parse(cmd);
	cout<<"parsed string is as follows"<<endl;
	for(i=0;;i++)
	{
		if(arg[i]==NULL)
		  break;
		cout<<arg[i]<<"--";
	}
	cout<<endl;
	return 0;
}
