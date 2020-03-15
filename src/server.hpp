#ifndef __HTTP_SERVER_CPP__
#define __HTTP_SERVER_CPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <sys/socket.h>

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

    void start();

private:
    int _port;
    int _backlog;

    uv_loop_t *loop;
    struct sockaddr_in addr;
    uv_stream_t *server;

    void setup();
    void listen();
};

#endif