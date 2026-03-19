#include "agent.config.h"
#include<iostream>
#include<fstream>
using namespace std;

static string trim(string s) {
    size_t b = s.find_first_not_of(" \t");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t");
    return s.substr(b, e - b + 1);
}

AgentConfig load_config(string config_path) {
    AgentConfig config;

    ifstream infile(config_path);
    if(!infile.is_open()) {
        cerr << "[aegis-agent] config not found: " << config_path << " (using defaults)\n";
        return config;
    }
    
    string line;
    while(getline(infile, line)) {
        auto comment = line.find('#');
        if(comment != string::npos) line = line.substr(0, comment);

        auto eq = line.find('=');
        if(eq == string::npos) continue;

        string key = trim(line.substr(0, eq));
        string value = trim(line.substr(eq + 1));

        if(key.empty() || value.empty()) continue;
        
        if(key == "agent_socket_path") {
            config.agent_socket_path = value;
        }else if(key == "agent_host_name") {
            config.host_name = value;
        }else if(key == "agent_control_url") {
            config.control_url = value;
        }
    }
    
    return config;
}