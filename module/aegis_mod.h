#ifndef __AEGIS_MOD_H__
#define __AEGIS_MOD_H__
#include "httpd.h"
#include "http_config.h"

/* 모듈 메타데이터 */
#define AEGIS_MODULE_NAME "aegis-mod"
#define AEGIS_MODULE_VERSION "0.0.2"

/* apache 디폴트 설정 */
#define AEGIS_DEFAULT_SOCKET "/var/run/aegis/agent.sock"
#define AEGIS_DEFAULT_TIMEOUT 200
#define AEGIS_DEFAULT_FAIL_OPEN 1
#define AEGIS_SOCKET_BUF_SIZE 8192

/* aegis-mod 설정 구조체 */
typedef struct {
    int timeout_ms; // agent 응답 최대 대기 시간 (ms)
    int fail_open; // agent의 오류가 발생할시 들어오는 요청들을 차단할지 말지
    const char *agent_socket; // 유닉스 소켓 경로
} aegis_server_cfg;

/* aegis-mod 디렉토리별 구성 */
typedef struct {
    int enabled; // 이 디렉토리는 검사할지 말지
} aegis_dir_cfg;

extern module AP_MODULE_DECLARE_DATA aegis_module;

#endif