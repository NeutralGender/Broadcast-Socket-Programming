#include "include/Client.h"

using namespace std;

int main()
{
    Client cli;

    cli.ActiveOpen();

    while(true)
    {
        cli.SendMsg();
        cli.ReceiveMsg();
    }

    return 0;
}
