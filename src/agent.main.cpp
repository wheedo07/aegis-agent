#include "agent.h"
#include<csignal>
AegisAgent *agent = new AegisAgent();

static void signal_handler(int sig) {
    printf("\n[aegis-agent] shutting down...\n");
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

    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    agent->start();

    return 0;
}