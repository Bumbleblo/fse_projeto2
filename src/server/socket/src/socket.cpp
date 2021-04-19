#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>
#include <mutex>

#include "socket.hpp"

#define PORT 8000

using namespace fsesocket;

void Socket::closeSocket()
{
    int code = close(socket_fd);

    if(code < 0){
        std::cerr << "Close error" << std::endl;
        throw SocketException();
    }
}

Socket::Socket(int port){

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    port = port;

    if(socket_fd==0){
        std::cerr << "Failed on server socket creation" << std::endl; 
        throw SocketException();
    }

    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt))< 0){
        std::cerr << "Set socket failure" << std::endl;

        throw SocketException();
    }


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int addrlen = sizeof(address);


    if(bind(socket_fd, (struct sockaddr*)&address, sizeof(address))<0){
        std::cerr << "Bind failure" << std::endl;
        throw SocketException();
    }

    if(listen(socket_fd, 20) < 0){
        std::cerr << "Listen fail" << std::endl;
        throw SocketException();
    }

}

int Socket::acceptConnection()
{
    int new_socket;
    int addrlen = sizeof(address);

    new_socket = accept(socket_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    if(new_socket < 0){
        std::cerr << "Fail to accept a new connection" << std::endl;
        throw SocketException();
    }

    return new_socket;
}


SocketClient::SocketClient(char *ip, int port)
{


    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd < 0){
        std::cerr << "Crate client socket error" << std::endl;
        throw SocketException();
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if(inet_pton(AF_INET, ip, &address.sin_addr) <= 0){
        std::cerr << "IP convert" << std::endl;
        throw SocketException();
    }

    int c_return = connect(socket_fd, (struct sockaddr*)&address, sizeof(address));

    if(c_return < 0){
        std::cerr << "Connection error" << std::endl;

        throw SocketException();
    }
}

void SocketClient::closeSocket()
{
    close(socket_fd);
}

void SocketClient::sendMessage(char *buffer, int size)
{
    send(socket_fd, buffer, size, MSG_EOR);

}

char* SocketClient::receiveMessage(int size, ssize_t* rsize)
{

    char *buffer = (char*)malloc(sizeof(char)*size);

    *rsize = recv(socket_fd, buffer, size, 0);


    return buffer;

}
