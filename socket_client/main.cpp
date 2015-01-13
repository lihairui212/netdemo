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

#define MAXDATASIZE 1024

#define SERVERIP "11.0.0.74"
#define SERVERPORT 15000

int main(int argc, char *argv[])
{
#ifdef WIN32
	WSADATA wsaData;
	int err = WSAStartup( MAKEWORD( 1, 1 ), &wsaData );//must be done
#endif

    char buf[MAXDATASIZE] ;
    int sockfd, numbytes;
    struct sockaddr_in server_addr;
    if ( (sockfd = socket(AF_INET,SOCK_STREAM , 0)) == - 1)
    {
        perror ( "socket error" ) ;
        return 1;
    }
    memset( &server_addr, 0, sizeof( struct sockaddr ) ) ;
    server_addr.sin_family = AF_INET ;
    server_addr.sin_port = htons( SERVERPORT) ;
    server_addr.sin_addr.s_addr = inet_addr( SERVERIP) ;
    if( connect( sockfd, (struct sockaddr *)&server_addr, sizeof( struct sockaddr ) ) == - 1)
    {
        perror ( "connect error" ) ;
        return 1;
    }
    printf ( "send: Hello, world!\n" ) ;
    if(send( sockfd, "Hello, world!" , 14, 0) == - 1)
    {
        perror ( "send error" ) ;
        return 1;
    }
    if( ( numbytes = recv( sockfd, buf, MAXDATASIZE, 0) ) == - 1)
    {
        perror ( "recv error" ) ;
        return 1;
    }
    if ( numbytes)
    {
        printf ( "received: %s\n" , buf) ;
    }
#ifdef WIN32
	shutdown(sockfd,2);
	closesocket(sockfd);
#else
	close(sockfd) ;
#endif
    return 0;
}
