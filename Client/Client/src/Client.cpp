#include "/root/Documents/CodeblocksProjects/S/Client/Client/include/Client.h"

void Client::ActiveOpen()
{
    try
    {
        if( (sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0 )
            throw ("Error, cannot create socket!\n");

        bzero(&serv,sizeof(serv));
        serv.sin_family = AF_INET;
        serv.sin_port = htons(5000);
        serv.sin_addr.s_addr = htonl(INADDR_ANY);

        struct timeval timeval;
        timeval.tv_sec = 1;
        timeval.tv_usec = 0;

        setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&timeval,sizeof(timeval));

    }
    catch(const char* except)
    {
        cout << except << std::endl;
        close(sockfd);
    }
    return;
}

int Client::ReceiveMsg()
{
    bzero(&rcv,sizeof(rcv));
    int nbytes = 0;

    try
    {
        if( (nbytes = recvfrom(sockfd,rcv,sizeof(rcv)-1,0,(struct sockaddr*)&serv,&serv_len)) < 0 && errno!=EWOULDBLOCK)
            throw ("Errro, no one bytes was received!\n");

        rcv[nbytes] = '\0';

        cout << rcv << std::endl;
    }
    catch(const char* except)
    {
        cout << except << std::endl;
        close(sockfd);
    }

    return (nbytes);
}

void Client::SendMsg()
{
    std::cin.getline(message_buf,MAXLINE);

    try
    {
        if( (sendto(sockfd,message_buf,sizeof(message_buf)-1,0,(struct sockaddr*)&serv,serv_len)) < 0 )
            throw ("Error, no one byte wasn`t send\n");
    }
    catch(const char* except)
    {
        cout << except << std::endl;
        close(sockfd);
    }

    return;
}


Client::~Client()
{
    close(sockfd);

}
