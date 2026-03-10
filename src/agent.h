#ifndef __AEGIS_AGENT_H__
#define __AEGIS_AGENT_H__
#include "agent.config.h"

#define AEGIS_AGENT_VERSION "0.0.1"
typedef struct {
    string control_url = "http://localhost:8080";
    AgentConfig config;
} AgentContext;

class AegisSocketServer;
class AegisAgent {
    private:
        bool running = false;
        AegisSocketServer* socket_server;

    public:
        AgentContext context;

    public:
        AegisAgent();
        ~AegisAgent();

        bool is_running() {
            return running;
        }

        void _ready();
        void _process();
};

#endif