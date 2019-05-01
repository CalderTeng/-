#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 8888

#define BACKLOG 2

int main()
{

	int listenfd, connectfd;
	struct sockaddr_in server;
	struct sockaddr_in client;
	socklen_t addrlen;
	
	//创建一个网络端点
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("error in listenfd.\n");
		exit(-1);
	}
	
	int opt = SO_REUSEADDR;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	//初始化
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//邦定端口和ip地址
	if(bind(listenfd, (struct sockaddr*)&server, sizeof(server)) == -1)
	{
		printf("error in bind\n");
		exit(-1);
	}
	
	//监听客户端的请求、
	if(listen(listenfd, BACKLOG) == -1)
	{
		printf("error in lsiten.\n");
		exit(-1);
	}

    while(1)
    {
	    //接受客户端的连接请求
	    if((connectfd = accept(listenfd, (struct sockaddr *)&client, &addrlen)) == -1)
	    {
		    printf("error in accept.\n");
		    exit(-1);
	    }
	    printf("客户端的ip是:%s\n端口号是：%d \n",inet_ntoa(client.sin_addr), ntohs(client.sin_port));

	    //发送数据
	    send(connectfd, "hello,\n",8,0);
	    close(connectfd);
	    close(listenfd);
	    printf("---------------------------------------\n");
	}
	
	return 0;
}

