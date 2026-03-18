#include "aegis_mod.h"
#include "aegis_mod.config.h"
#include "aegis_mod.collector.h"

/* 모듈 등록 */
static void aegis_register_hooks(apr_pool_t *p) {
    ap_hook_log_transaction(aegis_log_transaction, NULL, NULL, APR_HOOK_MIDDLE);
}

static const command_rec aegis_cmds[] = {
    AP_INIT_FLAG(
        "AegisEnabled",
        cfg_aegis_enabled, NULL,
        OR_AUTHCFG | ACCESS_CONF,
        "Enable Aegis request collection in this directory/location (On|Off)"
    ),
    AP_INIT_TAKE1(
        "AegisControlURL",
        cfg_control_url, NULL,
        RSRC_CONF,
        "URL of aegis-server that the agent will forward data to"
    ),
    AP_INIT_TAKE1(
        "AegisAgentSocket",
        cfg_agent_socket, NULL,
        RSRC_CONF,
        "Unix domain socket path for aegis-agent (default: " AEGIS_DEFAULT_SOCKET ")"
    ),
    AP_INIT_TAKE1(
        "AegisTimeout",
        cfg_timeout, NULL,
        RSRC_CONF,
        "Max wait time for agent in milliseconds (default: 200)"
    ),
    AP_INIT_FLAG(
        "AegisFailOpen",
        cfg_fail_open, NULL,
        RSRC_CONF,
        "Behavior when agent is unreachable: On=allow requests, Off=block requests"
    ),
    { NULL }
};

module AP_MODULE_DECLARE_DATA aegis_module = {
    STANDARD20_MODULE_STUFF,
    config_aegis_dir_create, // create per-dir config
    config_aegis_dir_merge, // merge per-dir config
    config_aegis_server_create, // create per-server config
    NULL,                       // merge per-server config
    aegis_cmds,
    aegis_register_hooks
};