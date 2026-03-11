#include "agent.h"
#include "agent.http.h"
#include "agent.socket.h"

AegisAgent::AegisAgent() {
    socket_server = nullptr;
}

AegisAgent::~AegisAgent() {}

void AegisAgent::start() {
    socket_server = new AegisSocketServer(context.config.agent_socket_path);
    http_server = new AegisHttpServer();
    socket_server->on_message([this](AegisMessage msg) {
    });

    if(socket_server->start()) printf("[aegis-agent] Agent successfully shut down\n");
}

void AegisAgent::stop() {
    if(socket_server) {
        socket_server->stop();
        delete socket_server;
        socket_server = nullptr;
    }
    if(http_server) {
        delete http_server;
        http_server = nullptr;
    }
}