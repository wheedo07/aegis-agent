#ifndef __AEGIS_AGENT_H__
#define __AEGIS_AGENT_H__
#include "agent.config.h"
#include "agent.api.h"

#define AEGIS_AGENT_VERSION "0.0.4"

struct AegisMessage;
class AegisSocketServer;
class AegisHttpServer;
class AegisAgent {
    public:
        AgentConfig config;

    private:
        AegisSocketServer *socket_server;
        AegisHttpServer *http_server;
        
        void ipset_block(std::string ip, int ttl);
        void register_agent();
        MessageResponse handle_message(AegisMessage msg);

    public:
        AegisAgent();
        ~AegisAgent();

        void start();
        void stop();
};

#endif