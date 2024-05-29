//Code::Blocks->Settings->Compiler->Linker->Other linker options: => "-lws2_32";
//static linking
#include "base64.h"
#include "AES.h"
#include <winsock2.h>
#include <iostream>
#include <process.h>
#include <vector>
using namespace std;
vector<string> message_list;
unsigned int __stdcall  listen_to_server(void *data);
bool InitWinSock2_0( ) ;
SOCKET SERVERSOCKET ;



char *SERVER_ADDR     = "41.225.145.184";//257.camdvr.org
const int BUFFER_SIZE = 2048;
const int SERVER_PORT = 257 ;



unsigned char iv[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

char* key_plain = "Knbhdf5aCtrjWGzpD93o0R8wm17xPlHdqsqdsqdqdq5sddsdsd";

/*unsigned char key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                     0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                     0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                     0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};*/
char* encrypt(char *message){
    unsigned char* converted_message = reinterpret_cast<unsigned char*>(message);
    unsigned char* converted_key = reinterpret_cast<unsigned char*>(key_plain);
    AES aes(AESKeyLength::AES_256);
    unsigned char *out = aes.EncryptCBC(converted_message, BUFFER_SIZE, converted_key, iv);
    char* outt = reinterpret_cast<char*>(out);
    return outt ;

}
char* decrypt(char *encypted){
    unsigned char* converted_encypted = reinterpret_cast<unsigned char*>(encypted);
    unsigned char* converted_key = reinterpret_cast<unsigned char*>(key_plain);
    AES aes(AESKeyLength::AES_256);
    unsigned char *innew = aes.DecryptCBC(converted_encypted, BUFFER_SIZE, converted_key, iv);
    char* outttt = reinterpret_cast<char*>(innew);
    return outttt ;
}

void replace_all(
    std::string& s,
    std::string const& toReplace,
    std::string const& replaceWith
) {
    std::string buf;
    std::size_t pos = 0;
    std::size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.find(toReplace, pos);
        if (pos == std::string::npos)
            break;
        buf.append(s, prevPos, pos - prevPos);
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(s, prevPos, s.size() - prevPos);
    s.swap(buf);
}

void CC(){
    #if defined _WIN32
        system("cls");
    #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        system("clear");
    #elif defined (__APPLE__)
        system("clear");
    #endif
}

void show_old_messages(){
    CC();for(int i=0;i<message_list.size();i++){cout << (message_list[i])  << endl;
    }
}

int connect(){
    closesocket( SERVERSOCKET ) ;
    WSACleanup( ) ;
    if ( ! InitWinSock2_0( ) )
        {
            cout << "Local Application Error !" << WSAGetLastError( ) << endl ;
            return -1 ;
        }
         while(1){
                CC();cout << "Connecting =>";Sleep(250);CC();cout << "Connecting ==>";Sleep(250);CC();cout << "Connecting ===>";Sleep(250);CC();cout << "Connecting ====>";Sleep(250);CC();cout << "Connecting =====>";Sleep(250);CC();cout << "Connecting ======>";Sleep(250);CC();cout << "Connecting =======>";Sleep(250);CC();cout << "Connecting ========>";
                SERVERSOCKET = socket(AF_INET,SOCK_STREAM,0);
                if ( SERVERSOCKET == INVALID_SOCKET )
                {
                    WSACleanup( ) ;
                    cout << "Local Application Error !" << WSAGetLastError( ) << endl ;
                    return -1 ;
                }
                struct hostent *hp = gethostbyname(SERVER_ADDR);
                if (hp != NULL) {SERVER_ADDR = inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[0]));}
                struct sockaddr_in serverAddr ;
                serverAddr . sin_family = AF_INET ;
                serverAddr . sin_addr . s_addr = inet_addr( SERVER_ADDR ) ;
                serverAddr . sin_port = htons( SERVER_PORT ) ;
                if ( connect( SERVERSOCKET, ( struct sockaddr * ) &serverAddr, sizeof( serverAddr ) ) >= 0 ){
                    _beginthreadex(0,0,listen_to_server,(void*)&SERVERSOCKET,0,0);
                    break;
                }
        }
}

unsigned int __stdcall listen_to_server(void *data)
{
    SOCKET* server =(SOCKET*)data;
    SOCKET server_socket = *server;
	char rec_buffer[BUFFER_SIZE];
	int r;
	while(r = recv( server_socket, rec_buffer, sizeof( rec_buffer ), 0 ))
	{

	    if(r < 1){connect();break;}
        rec_buffer[r] = '\0';
        if(strstr(rec_buffer, "---[SERVER]")){message_list.push_back((rec_buffer));}
        else{

        try{
            message_list.push_back/*base64_decode*/(((string)decrypt(rec_buffer)).erase (0,10));
        }
        catch (const runtime_error& error){
           message_list.push_back("---[LOCAL]---->This Message Is Damaged And Can't Be Displayed.");
        }

        }//
        show_old_messages();
	}
return 0;
}
bool InitWinSock2_0( )
{
    WSADATA wsaData ;WORD wVersion = MAKEWORD( 2, 0 ) ;if ( ! WSAStartup( wVersion, &wsaData ) )return true ;
    return false ;
}


int main()
{

    connect();
    char szBuffer[BUFFER_SIZE+1] ;
    szBuffer[ BUFFER_SIZE ] = '\0' ;
    int nLength = BUFFER_SIZE ;
    int nCntSend = 0 ;
    string input;string line;

    while (1){
        input = "";
        bool emptyinput = true;
        while (getline(cin, line))
        {
            if (line.empty()){
                    break;
            }else if(emptyinput == false){
                input += "\n";
            }
            if(!line.empty()){input += line;emptyinput = false;}

        }
            if(emptyinput == false){



                input = to_string(((rand()*21347)%10000000000) + 1000000000) + input;
                strcpy(szBuffer, (input.c_str()));
                char *pBuffer = szBuffer;
                while ( nCntSend = send(SERVERSOCKET, encrypt(pBuffer) , nLength, 0) != nLength){
                    if (nCntSend == -1){connect();}
                    if (nCntSend == nLength)
                        break ;
                    pBuffer += nCntSend;nLength -= nCntSend ;
                }
                strupr( szBuffer ) ;if ( strcmp( szBuffer, "QUIT" ) == 0 ){ break ;}//quit command
            }



    }
    closesocket( SERVERSOCKET ) ;
    WSACleanup( ) ;
    return 0 ;
}
