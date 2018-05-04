#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(int argc,char* argv[])
{
    //对命令行参数判定
    if(argc!=3)
    {
        printf("Usage:%s ,port,ip\n",argv[0]);
        return 1;
    }
    //创建套接字
    int sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0)
    {
        perror("socket");
        return 2;
    }
    //填充结构体
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(atoi(argv[2]));
    local.sin_addr.s_addr=inet_addr(argv[1]);
    //绑定
    if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
    {
        perror("bind");
        return 3;
    }
    //收发消息
    char buf[64];
    struct sockaddr_in client;
    while(1)
    {
        socklen_t len=sizeof(client);
        ssize_t s=recvfrom(sock,buf,sizeof(buf)-1,0,(struct sockaddr* )&client,&len);
        if(s>0)
        {
            buf[s]=0;
            printf("[%s:%d]:%s\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port),buf);
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&client,sizeof(client));
        }
    }
    return 0;
}
