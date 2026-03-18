#include "agent.h"
#include "agent.http.h"
#include "agent.socket.h"
#include<cstdio>
#include<iostream>

AegisAgent::AegisAgent() {
    socket_server = nullptr;
    http_server = nullptr;
}

AegisAgent::~AegisAgent() {}

void AegisAgent::start() {
    socket_server = new AegisSocketServer(context.config.agent_socket_path);
    http_server = new AegisHttpServer();

    socket_server->on_message([this](AegisMessage msg) {
        AegisHttpResponse result = http_server->handle_request(msg);
        if(result.block) {
            ipset_block(msg.ip, result.ttl);
            printf("[aegis-agent] Blocked IP: %s Reason: %s TTL: %ds\n", msg.ip.c_str(), result.reason.c_str(), result.ttl);
        }
    });
    if(socket_server->start()) printf("[aegis-agent] Agent successfully shut down\n");
}

void AegisAgent::ipset_block(string ip, int ttl) {
    string cmd = 
    "ipset add aegis-blocks " + ip + " timeout " + to_string(ttl) + " 2>/dev/null || " +
    "ipset add aegis-blocks " + ip + " timeout " + to_string(ttl);
    system(cmd.c_str());
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