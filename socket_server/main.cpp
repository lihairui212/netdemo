#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <Winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>

#include <errno.h>
#endif
#define MAXDATASIZE 80517//这个数值随着测试的接收的数据大小不同而改变

#define SERVERPORT 15000  //服务器端口
#define MAXCONN_NUM 10
#ifndef socklen_t
#define socklen_t int
#endif
int main(int argc, char *argv[])
{

#ifdef WIN32
	WSADATA wsaData;
	int err = WSAStartup( MAKEWORD( 1, 1 ), &wsaData );//must be done
#endif

	char buf[MAXDATASIZE] = {0x0} ;
    int  new_fd, numbytes;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sin_size;
   
	int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == - 1)
    {
        perror ( "socket error" ) ;
		int error = WSAGetLastError();
		printf("errno:%d\n",error);
        return 1;
    }
    memset( &client_addr, 0, sizeof(struct sockaddr)) ;
    server_addr.sin_family = AF_INET ;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)) == - 1)
    {
        perror ( "bind error") ;
		int error = WSAGetLastError();
        printf("errno:%d\n",error);
        return 1;
    }
    if( listen(sockfd, MAXCONN_NUM) == - 1)
    {
        perror ( "listen error" ) ;
        printf("errno:%d\n",errno);
        return 1;
    }

    while(1)
    {
        sin_size = sizeof(struct sockaddr_in) ;
        if (( new_fd = accept(sockfd,(struct sockaddr *)&client_addr, (socklen_t *)(&sin_size) )) == - 1)
        {
            perror ( "accept error" ) ;
            continue ;
        }
        printf ( "server: got connection from %s\n" , inet_ntoa(client_addr.sin_addr)) ;
		
	
		if ( ( numbytes = recv( new_fd, buf, MAXDATASIZE, 0)) == - 1)
		{
			perror ( "recv error" ) ;
			return 1;
		}
		printf ( "received: %s\n" , buf) ;
        printf ( "send: hi~~/n" ) ;
        if ( send(new_fd, "hi!!!" , 5, 0) == - 1)
        {
            perror( "send error" ) ;
            return 1;
        }
#ifdef WIN32
		shutdown(new_fd,2);
		closesocket(new_fd);
#else
        close(new_fd) ;
#endif
    }
    return 0;

}
