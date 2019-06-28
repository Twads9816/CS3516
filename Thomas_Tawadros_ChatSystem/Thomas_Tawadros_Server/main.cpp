#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <errno.h>

#define INVALID_NAME "/1"
#define VALID_NAME "/2"
#define NO_SLOTS "/3"

using namespace std;


struct user
{
    string name = "";
    int socket = NULL;
};

//list of users
user users[10];

int main()
{
    struct pollfd fds[200];
    int nfds = 1;
    int rec, i, j, k, l, on = 1;
    int current_size;
    int new_sock = -1;
    char buf[4096];
    bool close_conn = false;
    bool compress_array = false;
    struct sockaddr_in6 hint;
    bool end_server = false;
    string userCheck;
    bool doneRecv = false;
    bool updateUsers = false;

    //create a socket
    const int portnum = 54000;
    int listening = socket(AF_INET6, SOCK_STREAM, 0); //SOCK_STREAM specifies TCP socket being opened
    if (listening < 0)
    {
        cerr << "Can't create a socket!" << endl;
        return -1;
    }

    //allow socket to be reusable
    rec = setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rec < 0)
    {
        cout << "Failed to make socket reusable" << endl;
        close(listening);
        return -2;
    }

    //set socket to be non-blocking
    rec = ioctl(listening, FIONBIO, (char *)&on);
    if (rec < 0)
    {
        cout << "Failed to make the socket non-blocking" << endl;
        close(listening);
        return -3;
    }

    //fill in a hint structure
    memset(&hint, 0, sizeof(hint));
    hint.sin6_family = AF_INET6;
    memcpy(&hint.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    hint.sin6_port = htons(portnum); //host to network short translation to allow compatibility

    //bind the socket to an IP / port
    rec = bind(listening, (struct sockaddr*)&hint, sizeof(hint));
    if ( rec < 0)
    {
        cout << "Can't bind to IP/port" << endl;
        close(listening);
        return -4;
    }

    //mark the socket for listening in
    if (listen(listening, 32) < 0)
    {
        cerr << "Can't listen";
        close(listening);
        return -5;
    }
    else
    {
        cout << "The server is listening on port " << portnum << endl;

    }

    //initialize fd set for poll
    memset(fds, 0 , sizeof(fds));

    fds[0].fd = listening;

    fds[0].events = POLLIN;

    //never time out
    int timeout = -1;


    do
    {

        rec = poll(fds, nfds, timeout);

        //check if poll failed
        if (rec <0)
        {
            cout << "Poll failed" << endl;
            break;
        }

        //check if poll has timed out
        if (rec == 0)
        {
            cout << "Poll timed out" << endl;
            break;
        }

        //something is readable, go on to loop
        current_size = nfds;

        for (i = 0; i < current_size; i++)
        {
            //if not ready go to next fd
            if(fds[i].revents == 0)
                continue;

            if (fds[i].revents != POLLIN)
            {
                end_server = true;
                break;
            }
            if (fds[i].fd == listening)
            {
                //accept incoming connections
                do
                {
                    new_sock = accept(listening, nullptr, nullptr);
                    if (new_sock < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            cout << "Failed to accept incoming connect" << endl;
                            end_server = true;
                        }
                        break;
                    }
                    //if valid connection, add to fd list, mark for input, and increment number of fds
                    cout << "Client connected" << endl;
                    fds[nfds].fd = new_sock;
                    fds[nfds].events = POLLIN;
                    nfds++;
                } while (new_sock != -1);
            }
            //this is not the listening socket, therefore receive the information
            else
            {
                memset(buf, 0, 4096);

                close_conn = false;

                do
                {
                    //receive message
                    rec=recv(fds[i].fd, buf, sizeof(buf), 0);
                    if (rec < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            cout << "Failed to receive buffer" << endl;
                            close_conn = true;
                        }
                        break;
                    }
                    if  (rec == 0)
                    {
                        cout << "Connection closed" << endl;
                        close_conn = true;
                        break;
                    }

                    //data received
                    //if new username, check against user list
                    if (buf[0] == '/' && buf[1] == 'u')
                    {
                        userCheck = buf + 2;
                        cout << "Checking new user \"" << userCheck << "\" on port " << fds[i].fd << endl;

                        for (k = 0; k < 9; k++)
                        {
                            //if user already exists, reply with "user already exists"
                            if (users[k].name == userCheck)
                            {
                                userCheck = INVALID_NAME;
                                cout << "Username already exists" << endl;
                                rec = send(fds[i].fd, userCheck.c_str(), userCheck.size() + 1, 0);
                                break;
                            }
                        }
                        if (userCheck != INVALID_NAME)
                        {
                            //validate user
                            cout << "Valid username \"" << userCheck << "\" accepted"  << endl;

                            //add to user list
                            for (k = 0; k < 9; k++)
                            {
                                if (users[k].name == "")
                                {
                                    users[k] = user{userCheck, fds[i].fd};
                                    userCheck = VALID_NAME;
                                    rec = send(fds[i].fd, userCheck.c_str(), userCheck.size() + 1, 0);
                                    doneRecv = true;
                                    updateUsers = true;
                                    break;
                                }
                                else if (k >= 9)
                                {
                                    cout << "No empty user slots" << endl;
                                    userCheck = NO_SLOTS;
                                    rec = send(fds[i].fd, userCheck.c_str(), userCheck.size() + 1, 0);
                                }
                            }
                        }
                    }

                    //if a whisper, check if username is valid
                    else if (buf[0] == '(')
                    {
                        cout << "Whispering not implemented" << endl;
                    }
                    else
                    {
                        for (k = i+1; k < 9; k++)
                        {
                            string s(buf);
                            rec = send(fds[k].fd, s.c_str(), 4096, 0);
                            if (rec <= 0)
                            {
                                cout << "Port " << fds[k].fd << " did not receive a message" << endl;
                            }
                        }
                    }



                        //if so, send to user


                } while(!doneRecv);

                if (updateUsers)
                {
                    l = 1; //buffer position
                    memset(buf, 0, 4096);
                    buf[0] = '/';
                    for (k = 0; k < 9; k++)
                    {
                        //stack the usernames into the buffer
                        if(users[k].name != "")
                        {
                            string str = users[k].name;
                            l += sprintf(buf + l, "/u");
                            l += sprintf(buf + l, "%s", str);
                        }

                    }
                    string update(buf);
                    //send to all other users
                    for(k = 0; k < 9; k++)
                    {
                        if(users[k].name != "")
                        {
                            rec = send(users[k].socket, update.c_str(), update.size() + 1, 0);
                        }
                    }
                    updateUsers = false;
                }

                if (close_conn)
                {
                    //look for user with port of fds[i].fd and remove them from users
                    for(k = 0; k < 9; k++)
                    {
                        if(users[k].socket == fds[i].fd)
                        {
                            users[k] = {"", NULL};
                            break;
                        }
                    }
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    compress_array = true;
                }
            }
        }

        //compress the array as to get rid of unused socket descriptors
        if (compress_array)
        {
            compress_array = false;
            for (i = 0; i < nfds; i++)
            {
                if(fds[i].fd == -1)
                {
                    for(j = i; j < nfds; j++)
                    {
                        //change socket property of users
                        for(k = 0; k < 9; k++)
                        {
                            if (users[k].socket == fds[j].fd)
                            {
                                users[k].socket = fds[j+1].fd;
                            }
                        }
                        fds[j].fd = fds[j+1].fd;
                    }
                    i--;
                    nfds--;
                }
            }
        }
    } while (end_server == false);
}
/*
            int sock = socket(AF_INET, SOCK_STREAM, 0);

            sock = copy.

            if (FD_ISSET(listening, &copy)
            {
                //accept connection
                //display that the user has been connected to the other clients
                int client = socket(AF_INET, SOCK_STREAM, 0);
                client = accept(listening, nullptr, nullptr);

                //add the new connection to the list of connected clients
                FD_SET(client, &master);
                //add a user to the user list
                user newUser

                //send a welcome message to the connected client
                string connectedMsg = "You are now connected!";
                send(client, connectedMsg.c_str(), connectedMsg.size() + 1, 0);

                //Broadcast new connection
            }
            else
            {

                //receive message
                int bytesIn = recv(sock, buf, 4096, 0);
                if (bytesIn <= 0)
                {
                    //drop the client
                    closesocket(sock);
                    FD_CLR(sock, &master);
                    //remove from user list
                    //announce that the client has disconnected
                }
                else
                {
                    //check if the message is a whisper

                    //if not, broadcast it to other clients
                    for (int i = 0; i < master.fd_count; i++)
                    {
                        int outSock = socket(AF_INET, SOCK_STREAM, 0);

                        outSock = master.fd_array[i];
                        if(outSock != listening && outSock != sock)
                        {
                            ostringstream ss;
                            ss << "SOCKET #" << sock ": " << buf << "\r\n";
                            string strOut = ss.str();

                            send(outSock, strOut.c_str(), strOut.size() + 1, 0);
                        }
                    }
                }

            }
        }
    }

    return 0;
}
*/
