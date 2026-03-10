#include "agent.h"

int main(int argc, char *argv[]) {
    AegisAgent* agent = new AegisAgent();
    string config_path = AEGIS_AGENT_CONFIG_PATH;
    if(argc >= 3) {
        string arg = argv[1];
        if(arg == "--config" || arg == "-c") {
            config_path = argv[2];
        }
    } 
    agent->context.config = load_config(config_path);

    agent->_ready();
    while(agent->is_running()) {
        agent->_process();
    }
}