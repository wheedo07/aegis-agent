#ifndef __AEGIS_AGENT_CONFIG_H__
#define __AEGIS_AGENT_CONFIG_H__
#include<string>
using namespace std;

/* aegis-agent 설정 경로 */
#define AEGIS_AGENT_CONFIG_PATH "/etc/aegis/agent.conf"

/* 설정 기본값 */
#define CONFIG_DEFAULT_AGENT_SOCKET_PATH "/var/run/aegis/agent.sock"

typedef struct {
    string agent_socket_path = CONFIG_DEFAULT_AGENT_SOCKET_PATH;
} AgentConfig;

AgentConfig load_config(string config_path = AEGIS_AGENT_CONFIG_PATH);

#endif