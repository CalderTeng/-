#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>

#define PORT 8899

#define MAXSIZE 100


int main(int argc, char**argv)
{
    int sockfd, num;
    char buf[MAXSIZE];
    struct hostent* he;
    struct sockaddr_in server, peer;
    
    //判断输入的参数
    if(argc!=3)
    {
        printf("error:参数不够！\n");
        exit(-1);
    }
    
    //获取目标主机的相关信息
    if((he=gethostbyname(argv[1]))==NULL)
    {
        perror("Error in gethostbyname. \n");
        exit(-1);
    }
    //服务器初始化
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr = *((struct in_addr*)he->h_addr);
    
    //创建一个网络端点
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    {
        perror("Error in socket.\n");
        exit(-1);
    }
    
    //发送数据请求给服务器
    sendto(sockfd, argv[2], strlen(argv[2]),0,(struct sockaddr*)&server, sizeof(server));
    
    socklen_t len;
    len = sizeof(server);
    while(1)
    {
        //等到接收服务器的数据应答
        if((num=recvfrom(sockfd,buf,MAXSIZE,0,(struct sockaddr*)&peer,&len))==-1)
        {
            perror("Error in recvfrom.\n");
            exit(-1);
        }
        //判断接收的数据是否来自目标服务器
        if(len!=sizeof(server)||memcmp((const void*)&server,(const void*)&peer,len)!=0)
        {
            printf("收到的数据来自别的服务器.\n");
            continue;
        }
        
        buf[num] = '\0';
        printf("收到的数据是：%s.\n", buf);
        break;
    }
    close(sockfd);


    return 0;
}
