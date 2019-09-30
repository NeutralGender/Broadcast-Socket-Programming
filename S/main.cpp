#include "include/Server.h"

using namespace std;

int main(int argc, char *argv[])
{
    Server serv;
    serv.PassiveOpen();

    while(true)
    {
        unsigned short cport = serv.ReceiveMsg();

        if(!serv.Is_exist(cport))///check if client exists in client_info
        {
            string temp = serv.Register_client(cport);
            serv.Insert_Register(cport,temp);/// insert new client into client_info
        }
        else
        {
            serv.Parse_Receive_Msg();
            serv.Send_Switch(); ///STRING FOR INPUT: adress:message
                                ///EXAMPLE1: Broadcast: Hi everyone
                                ///EXAMPLE2: ALEX: MSG TO ALEX
        }

    }


    return 0;
}
