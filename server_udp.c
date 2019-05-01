#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define PORT 8899

#define MAXSIZE 100

int main()
{
    int sockfd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t len;
    int num;
    char buf[MAXSIZE];
    
    //创建一个网络端点
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    {
        printf("error in socket.\n");
        exit(-1);
    
    }
    
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //绑定ip地址和端口号
    if(bind(sockfd, (struct sockaddr*)&server, sizeof(server))==-1)
    {
        printf("error in bind.\n");
        exit(-1);
    }
    
    while(1)
    {
        //等待接受客户端的数据请求
        num = recvfrom(sockfd,buf,MAXSIZE,0,(struct sockaddr*)&client,&len);
        
        //对接受的数据进行处理
        if(num<0)
        {
            printf("error in recvfrom.\n");
            exit(-1);
        
        }
        buf[num] = '\0';
        
        printf("收到的数据是：%s\n客户端的ip是：%s\n端口号是：%d\n",buf,inet_ntoa(client. sin_addr),ntohs(client.sin_port));
        
        //向客户端发送应答数据
        sendto(sockfd,"welcome!\n",8,0,(struct sockaddr *)&client, len);
        
        if(!strcmp(buf, "bye"))
            break;
    }
    close(sockfd);
    
    return 0;
}
