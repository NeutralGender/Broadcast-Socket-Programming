#include "/root/Documents/CodeblocksProjects/S/S/include/Server.h"

void Server::PassiveOpen()
{
    try
    {
        if( (sockfd = socket(AF_INET,SOCK_DGRAM,0) ) < 0 )
            throw ("Socket() function error, Server cannot create socket!\n");

        bzero(&serv,sizeof(serv));///Nullify sock_addr_in structure, last 8 bytes must be Zero(sin_zero)
        serv.sin_family = AF_INET; ///IPv4 family
        serv.sin_port = htons(5000); ///Server port
        serv.sin_addr.s_addr = htonl(INADDR_ANY);

        if( (bind(sockfd,(struct sockaddr*)&serv,sizeof(serv)) ) < 0 )
            throw ("Bind() function error, Server cannot bind socket\n");
    }
    catch(const char* except)
    {
        cout << except << std::endl;
        close(sockfd);
    }

    return;
}

int Sendto(int sockfd,const char message_buffer[],struct sockaddr*addr,socklen_t len)
{
    return sendto(sockfd,message_buffer,sizeof(message_buffer),0,addr,len);
}


void Server::SendMsg()
{
    try
    {
        if(Sendto(sockfd,message_buffer,(struct sockaddr*)&cli,sizeof(cli)) <= 0)
        throw ("Error datagram sending, no one bytes was send!\n");
    }
    catch(const char* except)
    {
        cout << except << std::endl;
        close(sockfd);
    }

    return;
}

bool Server::Is_exist(unsigned short client_port)
{
    std::vector< std::pair<sockaddr_in,std::string > >::const_iterator it;
    for(it = client_info.begin(); it!=client_info.end();it++)
    {
        if(it->first.sin_port == cli.sin_port)
            return true;
    }

    return false;
}


unsigned short Server::ReceiveMsg()
{
    int nbytes = 0; ///bytes count for read

    try
    {
        socklen_t len = sizeof(cli);
        if( (nbytes = recvfrom(sockfd,message_buffer,sizeof(message_buffer),0,(struct sockaddr*)&cli,&len)) <= 0 )
            throw ("Error socket reading, no bytes can receive!\n");

        message_buffer[nbytes] = '\0';

        cout << "Message: " << message_buffer << endl;

        if( (host = gethostbyaddr((const char*)&cli.sin_addr.s_addr,sizeof(cli.sin_addr.s_addr),AF_INET)) == nullptr )
            throw ("Cannot get host by address!\n");

        cout << "Host: " << host->h_name << endl;

    }
    catch(const char* except)
    {
        cout << except << std::endl;
        close(sockfd);
    }

    return (ntohs(cli.sin_port));
}

void Server::Insert_Register(unsigned short cport,std::string message_buffer)
{
    client_info.push_back(std::make_pair(cli,message_buffer));

    ///ntohs() - network to host short, host`s byte order
    std::vector< std::pair<sockaddr_in,std::string > >::const_iterator it;
    for(it = client_info.cbegin();it!=client_info.cend();it++)
        cout << ntohs(it->first.sin_port) << " : " << it->second << std::endl;
}

std::string Server::Register_client(unsigned short cport)
{
    char hello_register[MAXLINE] = "Hello, please input your name for register\n";
    int nbytes = 0;
    socklen_t len = sizeof(cli);

    try
    {
        if( (sendto(sockfd,hello_register,sizeof(hello_register),0,(struct sockaddr*)&cli,sizeof(cli))) <= 0 )
        throw ("Error datagram sending, no one bytes was send!\n");

        if( (nbytes = recvfrom(sockfd,message_buffer,sizeof(message_buffer)-1,0,(struct sockaddr*)&cli,&len)) <= 0 )
        throw("Error, Registre receive client name\n");

        message_buffer[nbytes] = '\0';

        cout << "Client Register name: " << message_buffer << std::endl;
    }
    catch(const char* except)
    {
        cout << except << std::endl;
        close(sockfd);
    }

    return std::string(message_buffer);
}

void Server::Parse_Receive_Msg()
{
    std::string buf = static_cast<std::string>(message_buffer);

    size_t pos = buf.find(":");/// define ':' position

    address = buf.substr(0,pos);/// string before ':'
    msg = buf.substr(pos+1);/// to the end, without ':'

}


///FINDING CLIENT NAME BY HOST
std::string find_by_host(std::vector< std::pair<sockaddr_in,std::string > >client_info, int port)
{
    std::vector< std::pair<sockaddr_in,std::string > >:: const_iterator it;
    for(it = client_info.cbegin(); it != client_info.cend();it++)
    {
        if(ntohs(it->first.sin_port) ==  port)
            return it->second;
    }

    return nullptr;
}

///SEND MSG DEPENDING ON ADDRESS TYPE
void Server::Send_Switch()
{
    /// STRING FOR INPUT: adress:message

    std::string from = find_by_host(client_info,ntohs(cli.sin_port));
    msg = "From: " + from + ", Msg:" + msg;

    if(address == "Broadcast")
    {
        std::vector< std::pair<sockaddr_in,std::string > >:: const_iterator it;
        for(it = client_info.cbegin();it!=client_info.cend();it++)
        {
            sendto(sockfd,&msg[0],msg.size(),0,(struct sockaddr*)&it->first,sizeof(it->first));
            cout << "NOW SENDIG->" << ntohs(it->first.sin_port) << std::endl;

        }
    }
    else
    {
        std::vector< std::pair<sockaddr_in,std::string > >::const_iterator it;
        for(it = client_info.cbegin();it!=client_info.cend();it++)
        {
            if(it->second == address)
            {
                sendto(sockfd,&msg[0],msg.size(),0,(struct sockaddr*)&it->first,sizeof(it->first));
                cout << "NOW SENDIG->" << ntohs(it->first.sin_port) << std::endl;
            }

        }
    }
}

Server::~Server()
{
    close(sockfd);
    delete []host;
    host = nullptr;
}
