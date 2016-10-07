#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>

using namespace std;

int main()
{
	char cmd[1000],**arg=new char*[100];
	arg[0]=cmd;
	int pid,status;
	cout<<"enter the command please"<<endl;
	cin>>cmd;
	//strcpy(arg[0],cmd);
	pid=fork();
	if(pid==0)
	{
		if(execvp(arg[0],arg)==-1)
			cout<<"error"<<endl;
	}
	else
	{
		waitpid(pid,&status,WUNTRACED);
		cout<<"command executed"<<endl;
	}
	return 0;
}
