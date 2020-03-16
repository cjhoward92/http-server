#include "server.hpp"

/**
 * TODO:
 * We need to build an options object
 * We need to figure out the best way to handle routing/middleware
 * We need to not use the default loop, always (see options)
 * We need to spawn new loops for each request (maybe have a limit?)
 * We need to add an HTTP parser (picohttp?)
 */

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void on_close(uv_handle_t* handle) {
    free(handle);
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free_write_req(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    printf("Reading with status %ld\n", nread);
    if (nread > 0) {
        write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        uv_write((uv_write_t*) req, client, &req->buf, 1, echo_write);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, on_close);
    }

    free(buf->base);
}

uv_loop_t *HttpServer::_loop = uv_default_loop();

HttpServer::~HttpServer() {
    spdlog::debug("We are destroying\n");
}

int HttpServer::getBacklog() {
    return this->_backlog;
}

int HttpServer::getPort() {
    return this->_port;
}

int HttpServer::start() {
    spdlog::debug("Starting HTTP server\n");
    this->setup();
    return this->listen();
}

void HttpServer::setup() {
    // spdlog::debug("Setting up default loop");
    // HttpServer::_loop = uv_default_loop();

    spdlog::debug("Initializing TCP server");
    uv_tcp_init(HttpServer::_loop, &this->server);

    spdlog::debug("Allocating IPv4 addr");
    uv_ip4_addr("0.0.0.0", this->_port, &this->addr);

    spdlog::debug("Binding server");
    uv_tcp_bind(&this->server, (const struct sockaddr*)&this->addr, 0);
}

void HttpServer::onNewConnection(uv_stream_t *svr, int status) {
    spdlog::debug("New connection received");
    if (status < 0) {
        spdlog::error("New connection error %s\n", uv_strerror(status));
        // TODO: do something
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(HttpServer::_loop, client);
    if (uv_accept(svr, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*) client, on_close);
    }
}

int HttpServer::listen() {
    spdlog::debug("Starting to listen");
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, HttpServer::onNewConnection);
    if (r) {
        spdlog::error("Listen error %s\n", uv_strerror(r));
        return 1;
    }
    return uv_run(_loop, UV_RUN_DEFAULT);
}