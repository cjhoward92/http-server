#ifndef __HTTP_SERVER_CPP__
#define __HTTP_SERVER_CPP__

#include "stdhdr.hpp"

#define DEFAULT_PORT 8080
#define DEFAULT_BACKLOG 1024

class HttpServer {
public:
    HttpServer() : HttpServer(DEFAULT_PORT, DEFAULT_BACKLOG) {}
    HttpServer(int port) : HttpServer(port, DEFAULT_BACKLOG) {}
    HttpServer(int port, int backlog) : _port(port), _backlog(backlog) {
        printf("Port: %d - Backlog: %d\n", port, backlog);
    }

    ~HttpServer();

    int getPort();
    int getBacklog();

    int start();

private:
    int _port;
    int _backlog;

    struct sockaddr_in addr;
    uv_tcp_t server;

    void setup();
    int listen();
    
    static uv_loop_t *_loop;
    static void onNewConnection(uv_stream_t *svr, int status);
};

#endif