#include "stdhdr.hpp"
#include "server.hpp"

int main() {
    spdlog::debug("Hello darkness my old friend");

    HttpServer httpServer = HttpServer();
    int status = httpServer.start();

    spdlog::info("Service shutdown with %d", status);

    return status;
}