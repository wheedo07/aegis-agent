#include "agent.h"
#include "agent.socket.h"

AegisAgent::AegisAgent() {
    socket_server = nullptr;
}

AegisAgent::~AegisAgent() {}

void AegisAgent::_ready() {
    socket_server = new AegisSocketServer(context.config.agent_socket_path);
    socket_server->on_message([this](AegisMessage msg) {
    });
    running = socket_server->start();
}

void AegisAgent::_process() {
}