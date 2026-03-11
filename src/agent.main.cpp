#include "agent.h"
AegisAgent *agent = new AegisAgent();

static void signal_handler(int sig) {
    printf("[aegis-agent] received signal %d, shutting down...\n", sig);
    if(agent) agent->stop();
}

int main(int argc, char *argv[]) {
    string config_path = AEGIS_AGENT_CONFIG_PATH;
    if(argc >= 3) {
        string arg = argv[1];
        if(arg == "--config" || arg == "-c") {
            config_path = argv[2];
        }
    } 
    agent->context.config = load_config(config_path);
    agent->start();
}