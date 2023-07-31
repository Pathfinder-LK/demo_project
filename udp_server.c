#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

#define SERVER_PORT 8089

//打印输入提示选项
static void* usage(const char* port)
{ 
      printf("usage: %s [local_ip] [local_port]\n",port);
}

 int main(int argc,char* argv[])
 { 

      //创建套接字
      int server_sockfd = socket(AF_INET,SOCK_DGRAM,0);
      if(server_sockfd<0)
      {
          perror("socket");
          exit(1);
      }
    //将套接字与ip地址和端口号进行绑定
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;   
    if(argc > 1)
    {
        server_addr.sin_port = htons(atoi(argv[1]));
    }
    else
    {
        server_addr.sin_port = htons(SERVER_PORT);
    }
    // server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    // ip可是是本服务器的ip，也可以用宏INADDR_ANY代替，代表0.0.0.0，表明所有地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(server_sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        perror("bind");
        exit(2);
    }

    char buffer[4096];

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr); 
    
    
    while(1)
    { 
        //读取数据
        int r = recvfrom(server_sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&client_addr,&len);
        if(r<0)
        { 
            perror("recvfrom");
            exit(3);
        }

        // if (strcmp(buffer, "QUIT") == 0)
        // {
        //     printf("Client IP : %s  Port : %d has been closed\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
        //     memset(buffer, 0, sizeof(buffer));
        //     snprintf(buffer, sizeof(buffer), "%s", "tcp server will exit ... ...");
        //     // if(send(client_addr, buffer, strlen(buffer), 0) < 0)
        //     if(sendto(server_sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, len) < 0)
        //     {
        //         printf("send failed : %s \n", buffer);
        //     }
        // }
        printf("[%s %d]recv data : %s \n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), buffer);
        if((strcmp(buffer, "IP") == 0))
        {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s", inet_ntoa(client_addr.sin_addr));
            // if(send(client_addr, buffer, strlen(buffer), 0) < 0)
            if(sendto(server_sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, len) < 0)
            {
                printf("send failed : %s \n", buffer);
            }
        }
        else if((strcmp(buffer, "PORT") == 0))
        {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%d", htons(client_addr.sin_port));
            // if(send(client_addr, buffer, strlen(buffer), 0) < 0)
            if(sendto(server_sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, len) < 0)
            {
                printf("send failed : %s \n", buffer);
            }
        }
        else if((strcmp(buffer, "IP&PORT") == 0))
        {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "%s %d", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
            // if(send(client_addr, buffer, strlen(buffer), 0) < 0)
            if(sendto(server_sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, len) < 0)
            {
                printf("send failed : %s \n", buffer);
            }
        }
        else
        {
            // if(send(client_addr, buffer, strlen(buffer), 0) < 0)
            if(sendto(server_sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, len) < 0)
            {
                printf("send failed : %s \n", buffer);
            }
        }
        printf("[%s %d]send data : %s \n\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port), buffer);
        memset(buffer, 0, sizeof(buffer));
    }
    return 0;
 }


