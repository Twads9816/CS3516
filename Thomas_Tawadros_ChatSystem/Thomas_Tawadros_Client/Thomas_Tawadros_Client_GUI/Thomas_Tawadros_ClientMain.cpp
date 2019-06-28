/***************************************************************
 * Name:      Thomas_Tawadros_ClientMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Thomas Tawadros ()
 * Created:   2019-06-24
 * Copyright: Thomas Tawadros ()
 * License:
 **************************************************************/

#include "Thomas_Tawadros_ClientMain.h"
#include <wx/msgdlg.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <time.h>

extern Thomas_Tawadros_ClientFrame* MainFrame;

using namespace std;

char textbuf[200]; //for text input
bool isConn = false;
string userName = "";
int sock, i, j, k, on = 1;
char buf[4096]; //for server communication
char inbuf[4096]; //input from server
char inbuf2[200];
string name = "";
struct pollfd fds[200];
int nfds = 1;
int rec;



//(*InternalHeaders(Thomas_Tawadros_ClientFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(Thomas_Tawadros_ClientFrame)
const long Thomas_Tawadros_ClientFrame::ID_STATICTEXT1 = wxNewId();
const long Thomas_Tawadros_ClientFrame::ID_LISTBOX1 = wxNewId();
const long Thomas_Tawadros_ClientFrame::ID_STATICTEXT2 = wxNewId();
const long Thomas_Tawadros_ClientFrame::ID_LISTBOX2 = wxNewId();
const long Thomas_Tawadros_ClientFrame::ID_TEXTCTRL1 = wxNewId();
const long Thomas_Tawadros_ClientFrame::ID_PANEL1 = wxNewId();
const long Thomas_Tawadros_ClientFrame::ID_MENUITEM1 = wxNewId();
const long Thomas_Tawadros_ClientFrame::idMenuAbout = wxNewId();
const long Thomas_Tawadros_ClientFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Thomas_Tawadros_ClientFrame,wxFrame)
    //(*EventTable(Thomas_Tawadros_ClientFrame)
    //*)
END_EVENT_TABLE()

Thomas_Tawadros_ClientFrame::Thomas_Tawadros_ClientFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(Thomas_Tawadros_ClientFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxBoxSizer* BoxSizer3;
    wxMenu* Menu1;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer5;

    Create(parent, wxID_ANY, _("Client-Server Chat"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(300,274));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer5 = new wxBoxSizer(wxVERTICAL);
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Messages"), wxDefaultPosition, wxSize(66,17), 0, _T("ID_STATICTEXT1"));
    BoxSizer5->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    MessageBox = new wxListBox(Panel1, ID_LISTBOX1, wxDefaultPosition, wxSize(491,408), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    BoxSizer5->Add(MessageBox, 1, wxALL|wxEXPAND, 5);
    BoxSizer3->Add(BoxSizer5, 0, wxALL|wxEXPAND, 5);
    BoxSizer6 = new wxBoxSizer(wxVERTICAL);
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Connected Users"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer6->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    UserBox = new wxListBox(Panel1, ID_LISTBOX2, wxDefaultPosition, wxSize(159,408), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX2"));
    BoxSizer6->Add(UserBox, 1, wxALL|wxEXPAND, 5);
    BoxSizer3->Add(BoxSizer6, 0, wxALL|wxEXPAND, 5);
    BoxSizer2->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    TextCtrl1 = new wxTextCtrl(Panel1, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(680,27), wxTE_PROCESS_ENTER, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TextCtrl1->SetMaxLength(100);
    BoxSizer4->Add(TextCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(BoxSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel1->SetSizer(BoxSizer2);
    BoxSizer2->Fit(Panel1);
    BoxSizer2->SetSizeHints(Panel1);
    BoxSizer1->Add(Panel1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, ID_MENUITEM1, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    SetSizer(BoxSizer1);
    Layout();

    Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&Thomas_Tawadros_ClientFrame::OnTextCtrl1TextEnter);
    Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&Thomas_Tawadros_ClientFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&Thomas_Tawadros_ClientFrame::OnAbout);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Thomas_Tawadros_ClientFrame::OnClose);
    //*)
}

int connect()
{
    const string ipAddress = "::1"; //ipAddress of the server
    const int portnum = 54000; //Listening port # on the server

        //create socket
    sock = socket(AF_INET6, SOCK_STREAM, 0); //SOCK_STREAM specifies TCP socket being opened
    if (sock == -1)
    {
        //MessageBox->Append(wxString("Can't create a socket!"));
        return -1;
    }

    //fill in a hint structure
    sockaddr_in6 hint;
    hint.sin6_family = AF_INET6;
    hint.sin6_port = htons(portnum); //host to network short translation to allow compatibility
    hint.sin6_addr = in6addr_loopback;
    //connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == -1)
    {
        //MessageBox->Append(wxString("Can't connect to server"));
        close(sock);
        return -2;
    }
/*
    //set socket to be non-blocking
    int result = ioctl(sock, FIONBIO, (char *)&on);
    if (result < 0)
    {
    //MessageBox->Append(wxString("Failed to make the socket non-blocking"));
        close(sock);
        return -3;
    }
    */
    return 0;
}

void printg(string n, int t)
{
    if (t == 0)
    {
        MainFrame->UserBox->Append(wxString(n));
    }
    if (t == 1)
    {
        MainFrame->MessageBox->Append(wxString(n));
    }

}

//being listening for updates
int listen2()
{
    //set socket to be non-blocking
    int result = ioctl(sock, FIONBIO, (char *)&on);
    if (result < 0)
    {
    //MessageBox->Append(wxString("Failed to make the socket non-blocking"));
    printg("Failed to make socket non-blocking", 0);
        close(sock);
        return -3;
    }

    //allow socket to be reusable
    rec = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rec < 0)
    {
        printg("Failed to make socket reusable", 0);
        close(sock);
        return -4;
    }

    memset(fds, 0 , sizeof(fds));

    fds[0].fd = sock;

    fds[0].events = POLLIN;

    //update every second
    int timeout = (0 * 1000);


    do
    {

        rec = poll(fds, nfds, 0);
        if (rec == 0)
        {
            printg("Poll timed out", 1);
        }
        else
        {
            memset(inbuf, 0, 4096);
            if(fds[0].fd == sock)
            {
                rec = recv(fds[0].fd, inbuf, 4096, 0);
                if (rec > 0)
                {
                    string s(inbuf);
                    printg(s, 0);
                }
            }
        }
    //sleep(1);
    } while (false);
    return 0;
}

Thomas_Tawadros_ClientFrame::~Thomas_Tawadros_ClientFrame()
{
    //(*Destroy(Thomas_Tawadros_ClientFrame)
    //*)
}

void Thomas_Tawadros_ClientFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void Thomas_Tawadros_ClientFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void Thomas_Tawadros_ClientFrame::OnClose(wxCloseEvent& event)
{
    close(sock);
    event.Skip(true);
}

void Thomas_Tawadros_ClientFrame::OnTextCtrl1TextEnter(wxCommandEvent& event)
{
    //put input text into a variable
    memset(textbuf, 0, 200);
    wxString input = TextCtrl1 -> GetValue();

    TextCtrl1->SetValue(wxString(""));

    strncpy(textbuf, (const char*) input.mb_str(wxConvUTF8), 200);

    //is it a command?
    if(textbuf[0] == '/')
    {
        switch (textbuf[1])
        {
        case 'h' :
            MessageBox->Append(wxString("Use \"/c\" to connect to the server.\nUse \"(user)\" to whisper to a user."));
            break;
        case 'c' :
            if (isConn)
            {
                MessageBox->Append(wxString("Already connected"));
            }
            else
            {
                int result = connect();
                if (result == 0)
                {
                    MessageBox->Append(wxString("Enter \"/u\" followed by your username."));
                }
                else
                {
                    MessageBox->Append(wxString("Can't connect to the server"));
                }
            }
            break;
        case 'u' :

            //do we already have a username?
            if (userName != "")
            {
                MessageBox->Append(wxString("You already have a user name."));
                break;
            }
            char namebuf[200];
            int j = 0;

            //loop to extract name from input buffer
            //eliminates white-space and tabs, and adds terminating char
            for (int i = 2; i < 199; i++)
            {
                if (textbuf[i] == '\0')
                {
                    namebuf[j] = '\0';
                    break;
                }
                else if (!isspace(textbuf[i]))
                {
                    namebuf[j] = textbuf[i];
                    j++;
                }

            }
            //check if valid user name
            if (j == 0)
            {
                MessageBox->Append(wxString("Invalid username"));
            }
            else
            {
                //check against other usernames on the server
                string name (namebuf);
                string userCheck = "/u";
                userCheck.append(name);
                memset(buf, 0, 4096);

                int result = send(sock, userCheck.c_str(), userCheck.size() + 1, 0);

                if (result < 0)
                {
                    MessageBox->Append(wxString("Failed to send"));
                }
                else
                {
                    //wait for confirmation from server
                    result = recv(sock, buf, sizeof(buf) + 1, 0);
                    {
                        if (result <= 0)
                        {
                            MessageBox->Append(wxString("Failed to receive user name confirmation"));
                            break;
                        }
                        else
                        {
                            //if duplicate name, reply username already exists
                            if (buf[0] == '/' && buf[1] == '1')
                            {
                                MessageBox->Append(wxString("Username already exists, try a different one."));
                                break;
                            }
                            else if (buf[0] == '/' && buf[1] == '2')
                            {
                            //if not a duplicate, admit into server
                                string output = "Username: ";
                                output.append(name);
                                userName = name;
                                MessageBox->Append(wxString("Welcome to the server!"));
                                MessageBox->Append(wxString(output));
                                //being listening for updates
                                listen2();
                            }
                            else if (buf[1] == '3')
                            {
                                MessageBox->Append(wxString("No empty slots, try again later"));
                            }

                        }
                    }

                }

            }
            break;
        }
    }
    //if not command, is it a whisper?
    else if(textbuf[0] == '(')
    {
        if(userName == "")
        {
            MessageBox->AppendAndEnsureVisible(wxString("You must first enter a username"));
        }
        MessageBox->AppendAndEnsureVisible(wxString("Function not implemented"));
    }
    //if neither command nor whisper, then regular message
    else
    {
        if(userName == "")
        {
            MessageBox->AppendAndEnsureVisible(wxString("You must first enter a username"));
        }
        else
        {
            string s(textbuf);
            rec=send(sock, s.c_str(), 200, 0);
            if (rec <= 0)
            {
                MessageBox->AppendAndEnsureVisible(wxString("The message was not send"));
            }
        }
    }
}

