#include "server.hpp"

/**
 * TODO:
 * We need to build an options object
 * We need to figure out the best way to handle routing/middleware
 * We need to not use the default loop, always (see options)
 * We need to spawn new loops for each request (maybe have a limit?)
 * We need to add an HTTP parser (picohttp?)
 */

HttpServer::~HttpServer() {
    printf("We are destroying\n");
}

int HttpServer::getBacklog() {
    return this->_backlog;
}

int HttpServer::getPort() {
    return this->_port;
}

void HttpServer::start() {
    printf("Starting HTTP server\n");
}

void HttpServer::setup() {

}

void HttpServer::listen() {

}