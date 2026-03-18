#ifndef __AEGIS_AGENT_H__
#define __AEGIS_AGENT_H__
#include "agent.config.h"
#include "agent.api.h"
using namespace std;

#define AEGIS_AGENT_VERSION "0.0.3"
typedef struct {
    string control_url = "http://localhost:8080";
    AgentConfig config;
} AgentContext;

class AegisSocketServer;
class AegisHttpServer;
class AegisAgent {
    public:
        AgentContext context;

    private:
        AegisSocketServer *socket_server;
        AegisHttpServer *http_server;

        void ipset_block(string ip, int ttl);
        MessageResponse handle_message(AegisMessage msg);

    public:
        AegisAgent();
        ~AegisAgent();

        void start();
        void stop();
};

#endif