#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
/*structure Definition*/
struct info{
	char cmd[200];
	char buf[1000];
	char filename[100];
	int  size;
	int  ack;
};
/*structure declaration*/
struct info obj;
struct stat a;

socklen_t clilen;
struct sockaddr_in cliaddr,servaddr;
int ret_val, connfd, sockfd, fd;
pthread_t th;
ssize_t ret;

void *data_connection(void *arg){
	printf("start\n");
	char temp[]=" >txt.txt";

	/*commands receive from client*/
	read(connfd,&obj,sizeof(obj));
	printf("%s\n",obj.cmd);
	/*command to change directory*/
	if(strcmp(obj.cmd,"cd") == 0){
		chdir(obj.filename);
		char s[100];
		printf("%s\n",getcwd(s,100));
		goto pwd;
		printf("changed path successfully\n");
		obj.ack=1;
		write(connfd,&obj,sizeof(obj));
		}
	/*command to diplay the current directory*/
	else if(strcmp(obj.cmd, "pwd") == 0){
		pwd:
			system("pwd>txt.txt");
			fd = open("./txt.txt",O_RDONLY);
			stat("./txt.txt",&a);
			obj.size = a.st_size;
			read(fd, obj.buf, obj.size);
			printf("%s",obj.buf);
			write(connfd, &obj, sizeof(obj));
	}
	/*command to file names in current directory*/
	else if(strcmp(obj.cmd, "dir") ==0){
		system("dir>txt.txt");
		fd = open("./txt.txt",O_RDONLY);
		stat("./txt.txt",&a);
		obj.size = a.st_size;
		read(fd, obj.buf,obj.size);
		write(connfd, &obj, sizeof(obj));
	}
	/*command to download a file*/
	else if(strcmp(obj.cmd,"get")==0){
		stat(obj.filename,&a);
		obj.size=a.st_size;

		write(connfd,&obj,sizeof(obj));

		read(connfd,&obj,sizeof(obj));
		if(obj.ack==1){
			fd=open(obj.filename,O_RDONLY);
			printf("sending the file\n");
			ret = splice(fd , 0, connfd, NULL, obj.size, SPLICE_F_MOVE);
	
		//	sendfile(connfd, fd, NULL, obj.size);
		}

		
	}
	/*command to uplaod a fle*/
	else if(strcmp(obj.cmd,"put") == 0){
		fd = open(obj.filename,O_CREAT | O_EXCL | O_WRONLY);
		if(fd<0){
			perror("fail to create and open the file\n");
		}
 		char *ptr;
                ptr = malloc(obj.size);
                obj.ack=1;
                write(connfd, &obj, sizeof(obj));
                int ret = read(connfd,ptr,obj.size);
                if(ret < 0){
                      perror("read failed\n");
		      obj.ack = 0;
		      write(connfd, &obj, sizeof(obj));
		}
		
		write(fd, ptr, obj.size);
		close(fd);
		write(connfd, &obj, sizeof(obj));
	}
	
	else if(strcmp(obj.cmd, "quit") == 0){
		printf("myftp client is quitted\n");
		obj.ack = 1;
		write(connfd, &obj, sizeof(obj));
	}
}


int main(){


		/*create a TCP/IP socket*/	
		sockfd=socket(AF_INET,SOCK_STREAM,0);

		bzero(&servaddr,sizeof(servaddr));
		servaddr.sin_family=AF_INET;
		servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
		servaddr.sin_port=htons(8000);

		/*Assign address to socket*/	
		ret_val=bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

		/*Enable protocol operation*/	
		listen(sockfd,5);
		/*Accepting a connection with client*/
		clilen=sizeof(cliaddr);

		connfd=accept(sockfd,(struct sockaddr *)&cliaddr,&clilen);

		printf("client port no = %d\n",ntohs(cliaddr.sin_port));
		printf("server port no = %d\n",ntohs(servaddr.sin_port));
		while(1){
			/*Creating a thread*/
			pthread_create(&th,NULL,data_connection,NULL);					pthread_join(th,NULL);
			printf("return\n");
			}
	return 0;
}
