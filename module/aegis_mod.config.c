#include "aegis_mod.config.h"
#include "aegis_mod.h"

void *config_aegis_server_create(apr_pool_t *p, server_rec *s) {
    aegis_server_cfg *cfg = apr_pcalloc(p, sizeof(aegis_server_cfg));
    cfg->agent_socket = AEGIS_DEFAULT_SOCKET;
    cfg->timeout_ms = AEGIS_DEFAULT_TIMEOUT;
    cfg->fail_open = AEGIS_DEFAULT_FAIL_OPEN;
    return cfg;
}
void *config_aegis_dir_create(apr_pool_t *p, char *dir) {
    aegis_dir_cfg *cfg = apr_pcalloc(p, sizeof(aegis_dir_cfg));
    cfg->enabled = 0;
    return cfg;
}
void *config_aegis_dir_merge(apr_pool_t *p, void *base, void *add) {
    aegis_dir_cfg *parent = (aegis_dir_cfg*) base;
    aegis_dir_cfg *child = (aegis_dir_cfg*) add;
    aegis_dir_cfg *merged = apr_pcalloc(p, sizeof(aegis_dir_cfg));

    /* 자식 디렉토리에 명시된 값 우선 */
    merged->enabled = child->enabled ? child->enabled : parent->enabled;
    return merged;
}



const char *cfg_aegis_enabled(cmd_parms *cmd, void *dcfg, int flag) {
    aegis_dir_cfg *dir = (aegis_dir_cfg*) dcfg;
    dir->enabled = flag;
    return NULL;
}
const char *cfg_agent_socket(cmd_parms *cmd, void *dcfg, const char *path) {
    aegis_server_cfg *cfg = ap_get_module_config(cmd->server->module_config, &aegis_module);
    cfg->agent_socket = apr_pstrdup(cmd->pool, path);
    return NULL;
}
const char *cfg_timeout(cmd_parms *cmd, void *dcfg, const char *val) {
    aegis_server_cfg *cfg = ap_get_module_config(cmd->server->module_config, &aegis_module);
    int ms = atoi(val);
    if(ms <= 0) return "AegisTimeout must be a positive integer";
    cfg->timeout_ms = ms;
    return NULL;
}
const char *cfg_fail_open(cmd_parms *cmd, void *dcfg, int flag) {
    aegis_server_cfg *cfg = ap_get_module_config(cmd->server->module_config, &aegis_module);
    cfg->fail_open = flag;
    return NULL;
}