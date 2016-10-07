else if(k==count-1)
			{
	//			cout<<"am i coming here?"<<endl;
				//last command, reads from pipe number k-1 and writes to output
				close(0);
				int r=dup2(fd[k-1][0],0);
				if(r==-1)
				{
					perror("dup2 error");
					exit(0);
				}
				close(fd[k][0]); //close the read end
				close(1);
				int w=dup2(fd[k][1],1);
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
			
