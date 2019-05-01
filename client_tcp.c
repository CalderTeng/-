#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>


#define PORT 8888
#define MAXSIZE 100

int main(int argc, char**argv)
{
    int sockfd, num;
    
    //存储字符的缓冲区
    char buf[MAXSIZE];
    
   //指向目的主机的指针
    struct hostent *he;
    
   //服务器信息 
    struct sockaddr_in server;
    
    if(argc != 2)
    {
        printf("参数不足！\n");
        exit(-1);
    
    }
    
    //获取要连接服务器的主机信息
    if((he=gethostbyname(argv[1]))==NULL)
    {
    
        printf("Error in gethostbyname.\n");
        exit(-1);
    
    }
    
    //创建一个网络端点
     if((sockfd=socket(AF_INET, SOCK_STREAM, 0))==-1)
     {
        printf("Error in socket.\n");
        exit(-1);
     }
     
     //初始化主机信息，并进行赋值
     bzero(&server, sizeof(server));
     server.sin_family = AF_INET;
     server.sin_port = htons(PORT);
     server.sin_addr = *((struct in_addr *)he -> h_addr);
     
     //向服务器发出连接请求
     if(connect(sockfd, (struct sockaddr *)&server, sizeof(server))==-1)
     {
     
        printf("Error in connect.\n");
        exit(-1);
     }
     
     //接收服务器端发来的数据，返回的是接收的字节数
     if((num=recv(sockfd,buf,MAXSIZE,0))==-1)
     {
        printf("Error in recv.\n");
        exit(-1);
     }
     buf[num] = '\0';
     printf("接收的信息是：%s", buf);
     close(sockfd);
     
    return 0;
}
