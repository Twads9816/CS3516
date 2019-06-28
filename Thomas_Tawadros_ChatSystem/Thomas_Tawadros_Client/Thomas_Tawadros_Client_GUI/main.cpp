#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main()
{
    const string ipAddress = "127.0.0.1"; //ipAddress of the server
    const int portnum = 54000; //Listening port # on the server

    //create socket
        int sock = socket(AF_INET6, SOCK_STREAM, 0); //SOCK_STREAM specifies TCP socket being opened
    if (sock == -1)
    {
        cerr << "Can't create a socket!";
        return -1;
    }

    //fill in a hint structure
        sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(portnum); //host to network short translation to allow compatibility
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr); //ip to string of integers

    //connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == -1)
    {
        cerr << "Can't connect to server" << endl;
        close(sock);
        return -2;
    }

    //do-while loop to send and receive data
    char buf[4096];
    string userInput;

    do
    {
        //prompt the user for text
        cout <<"> ";
        getline(cin, userInput);

        if (userInput.size() > 0) //make sure user typed in something
        {
        //send the text
        int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0); //add 1 for terminating 0
        if (sendResult != -1)
        {
            //wait for response
            memset(buf, 0, 4096);
            int bytesReceived = recv(sock, buf, 4096, 0);
            if (bytesReceived > 0)
            {
                //echo response to console
                cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
            }
        }
        }


    } while (userInput.size() > 0);

    //shutdown
    close(sock);

    return 0;
}
