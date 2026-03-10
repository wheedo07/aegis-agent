#ifndef __AEGIS_MOD_CONFIG_H__
#define __AEGIS_MOD_CONFIG_H__
#include "httpd.h"
#include "http_config.h"
#include "ap_config.h"

/* 설정 생성 및 병합 */
void *config_aegis_server_create(apr_pool_t *p, server_rec *s);
void *config_aegis_dir_create(apr_pool_t *p, char *dir);
void *config_aegis_dir_merge(apr_pool_t *p, void *base, void *add);

/* 지시자 핸들러 setter */
const char *cfg_aegis_enabled(cmd_parms *cmd, void *dcfg, int flag);
const char *cfg_control_url(cmd_parms *cmd, void *dcfg, const char *url);
const char *cfg_agent_socket(cmd_parms *cmd, void *dcfg, const char *path);
const char *cfg_timeout(cmd_parms *cmd, void *dcfg, const char *val);
const char *cfg_fail_open(cmd_parms *cmd, void *dcfg, int flag);

#endif