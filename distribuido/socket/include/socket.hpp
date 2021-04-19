#ifndef __SOCKET__
#define __SOCKET__

#include <sys/socket.h>
#include <netinet/in.h>


namespace fsesocket{


class SocketException: public std::exception{

    virtual const char* what() const throw()
    {
        return "Socket Error";
    }

};

class Socket{

    private:

        int socket_fd;
        int opt;
        struct sockaddr_in address;

    public:
        Socket(int port);

        int acceptConnection();
        void closeSocket();

};

class SocketClient{
    private:
        int socket_fd;
        int opt;
        struct sockaddr_in address;

    public:
        SocketClient(char *ip, int port);

        void sendMessage(char *buffer, int size);
        char* receiveMessage(int size, ssize_t *rsize);
        void closeSocket();
};

}

#endif
