#ifndef __AEGIS_AGENT_API_H__
#define __AEGIS_AGENT_API_H__

typedef struct {
    bool block = false;
    string reason;
    int ttl = 300; // 차단 유지 시간(초)
} MessageResponse;

/* Control 서버의 API 엔드포인트 */
#define AEGIS_HTTP_API_CHECK "/api/agent/check"
#define AEGIS_HTTP_API_REGISTER "/api/agent/register"

#endif