#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 256

using std::cout;

class Client
{
    public:
        Client() : sockfd(0),serv_len(sizeof(serv)) {} ;
        void ActiveOpen();
        std::string Register();
        int ReceiveMsg();
        void SendMsg();
        virtual ~Client();

    protected:

    private:
    struct sockaddr_in serv,cli;
    int sockfd;
    socklen_t serv_len;
    char message_buf[MAXLINE];
    char rcv[MAXLINE];

};

#endif // CLIENT_H
