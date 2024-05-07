#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
 
int main(int argc,char *argv[])
{
    //创建无名管道
	int pipefd[2];
	if(pipe(pipefd)){					
		perror("pipe error");
		exit(-1);
	}
	printf("[0] %d \n",pipefd[0]);    //pipefd[0]：读
	printf("[1] %d \n",pipefd[1]);    //pipefd[1]: 写
	
	//创建进程
	pid_t pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}else if(pid == 0){
		//子进程读取父进程写入的数据
		//close(pipefd[1]);	//子进程只读取数据，所以把写关闭
		int client;
        read(pipefd[0], &client, sizeof(client));//读取数据
		printf("父进程：%d\n",client);
	}else{
		//父进程写入数据
		//close(pipefd[0]);//父进程只写数据，所以把读关闭
		int c = 1234;	
		write(pipefd[1],&c,sizeof(c));//父进程写数据到pipefd[1]里
	}
	
	return 0;
}
