#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512
int main(int, char **)
{
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    std::cout << "Hello, from CSocketClient!\n";
    /**
     * 解析服务器地址和端口
     * 调用getaddrinfo函数将服务器的地址("127.0.0.1")和端口号(DEFAULT_PORT)
     * 解析为一个或多个addrinfo 结构体,并存储在result指向的链表中。
     */
    iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);

    if (iResult != 0) // 检查 getaddrinfo 函数的返回值，如果返回值不为0，则表示解析失败。
    {
        std::cerr << "getaddrinfo failed: " << iResult << std::endl; // 输出错误代码
        WSACleanup();                                                // 函数清理Windows Sockets库的资源
        return 1;
    }
    SOCKET ConnectSocket = INVALID_SOCKET; // 定义一个SOCKET类型的遍历
    // 尝试连接到服务器
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        // 创建套接字
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
            continue;
        }
        // 连接到服务器
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    };
    freeaddrinfo(result);
    if(ConnectSocket==INVALID_SOCKET)
    {
        std::cerr<<"Unable to connect to server!"<<std::endl;
        WSACleanup();
        return 1;
    }
    char revbuf[DEFAULT_BUFLEN];
    iResult=recv(ConnectSocket,revbuf,DEFAULT_BUFLEN,0);
    if(iResult>0)
    {
        std::cout<<"Received: "<<std::string(revbuf,0,iResult)<<std::endl;
    }else if(iResult==0)
    {
        std::cout<<"Connection closed"<<std::endl;
    }else{
        std::cerr<<"recv failed: "<<WSAGetLastError()<<std::endl;
    }
    //关闭套接字
    iResult = shutdown(ConnectSocket,SD_SEND);
    if(iResult==SOCKET_ERROR)
    {
        std::cerr<<"shutdown failed:"<<WSAGetLastError()<<std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}
