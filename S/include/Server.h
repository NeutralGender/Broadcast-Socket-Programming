#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <vector>
#include <algorithm>

#define MAXLINE 256

using std::cout;
using std::endl;

class Server
{
    public:
        Server() : sockfd(0),host(nullptr){};
        void PassiveOpen();
        unsigned short ReceiveMsg();
        void SendMsg();
        void Parse_Receive_Msg();
        bool Is_exist(unsigned short);
        std::string Register_client(unsigned short);
        void Insert_Register(unsigned short,std::string);
        void Send_Switch();
        virtual ~Server();

    protected:

    private:
    struct sockaddr_in serv,cli;
    struct hostent *host;///host structure
    int sockfd;///socket descriptor
    char message_buffer[MAXLINE];
    std::string address = "";
    std::string msg = "";

    std::vector< std::pair<sockaddr_in,std::string > > client_info;

};

#endif // SERVER_H
