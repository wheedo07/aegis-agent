#ifndef __AEGIS_AGENT_CONFIG_H__
#define __AEGIS_AGENT_CONFIG_H__
#include<string>

/* aegis-agent 설정 경로 */
#define AEGIS_AGENT_CONFIG_PATH "/etc/aegis/agent-conf/agent.conf"

/* 설정 기본값 */
#define CONFIG_DEFAULT_AGENT_SOCKET_PATH "/var/run/aegis/agent.sock"
#define CONFIG_DEFAULT_AGENT_HOST_NAME "ErrorHost"
#define CONFIG_DEFAULT_AGENT_CONTROL_URL "http://localhost:8080"

struct AgentConfig {
    std::string control_url = CONFIG_DEFAULT_AGENT_CONTROL_URL;
    std::string host_name = CONFIG_DEFAULT_AGENT_HOST_NAME;
    std::string agent_socket_path = CONFIG_DEFAULT_AGENT_SOCKET_PATH;
};

AgentConfig load_config(std::string config_path = AEGIS_AGENT_CONFIG_PATH);

#endif