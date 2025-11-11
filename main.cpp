#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#pragma comment(lib,"Ws2_32.lib")
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
int main(int, char**){
    WSADATA wsaData;
    int iResult;
    iResult=WSAStartup(MAKEWORD(2,2),&wsaData);
    if(iResult!=0)
    {
        std::cerr<<"WSAStartup failed: "<<iResult<<std::endl;
        return 1;
    }
    struct addrinfo *result=NULL,*ptr=NULL,hints;
    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;
    std::cout << "Hello, from CSocketClient!\n";
}
