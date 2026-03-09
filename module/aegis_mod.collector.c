#include "aegis_mod.collector.h"
#include "aegis_mod.h"

#include "http_log.h"
#include "http_request.h"
#include "apr_strings.h"
#include "apr_time.h"
#include<sys/socket.h>
#include<sys/un.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

static void sanitize_value(char *dst, const char *src, size_t dst_size) {
    if(!src) {
        dst[0] = '\0';
        return;
    }
    size_t i = 0;
    while(*src && i < dst_size - 1) {
        char c = *src++;
        dst[i++] = (c == '\n' || c == '\r') ? ' ' : c;
    }
    dst[i] = '\0';
}

/*
요청 정보를 "key=value\n" 형식으로 buf 에 기록한다.
마지막 빈 줄(\n)이 메시지 끝 마커 역할을 한다.
반환: 기록된 바이트 수, 버퍼 부족 시 -1
*/
static int build_message(request_rec *r, const aegis_server_cfg *cfg, char *buf, size_t buf_size) {
    char ua[512], host[256], ct[256], cl[32], ref[512], xff[256];
    const char *client_ip = r->useragent_ip ? r->useragent_ip : r->connection->client_ip;

    sanitize_value(ua,   apr_table_get(r->headers_in, "User-Agent"),      sizeof(ua));
    sanitize_value(host, apr_table_get(r->headers_in, "Host"),            sizeof(host));
    sanitize_value(ct,   apr_table_get(r->headers_in, "Content-Type"),    sizeof(ct));
    sanitize_value(cl,   apr_table_get(r->headers_in, "Content-Length"),  sizeof(cl));
    sanitize_value(ref,  apr_table_get(r->headers_in, "Referer"),         sizeof(ref));
    sanitize_value(xff,  apr_table_get(r->headers_in, "X-Forwarded-For"), sizeof(xff));

    int n = snprintf(buf, buf_size,
        "version=%s\n"
        "ip=%s\n"
        "method=%s\n"
        "uri=%s\n"
        "host=%s\n"
        "query=%s\n"
        "user_agent=%s\n"
        "content_type=%s\n"
        "content_length=%s\n"
        "referer=%s\n"
        "x_forwarded_for=%s\n"
        "protocol=%s\n"
        "control_url=%s\n"
        "timestamp=%ld\n"
        "\n",
        AEGIS_MODULE_VERSION,
        client_ip ? client_ip : "",
        r->method ? r->method : "",
        r->uri ? r->uri : "",
        host,
        r->args ? r->args : "",
        ua, ct, cl, ref, xff,
        r->protocol ? r->protocol : "",
        cfg->control_url ? cfg->control_url : "",
        (long)apr_time_sec(apr_time_now())
    );

    if(n <= 0 || (size_t)n >= buf_size) return -1;
    return n;
}

/*
ENOENT == 소켓 파일 없음 (에이전트 미실행) : 무시
EAGAIN == 에이전트 수신 버퍼 가득 참 : 무시
그 외  == DEBUG 레벨 로그만 남기고 무시
*/
static void send_agent(request_rec *r, const aegis_server_cfg *cfg, const char *buf, int len) {
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(fd < 0) {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "aegis-mod: socket() failed: %s", strerror(errno));
        return;
    }

    /* 소켓 자체를 비블로킹으로 설정 (sendto 의 MSG_DONTWAIT 과 이중 보호) */
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags >= 0) fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, cfg->agent_socket, sizeof(addr.sun_path) - 1);

    ssize_t sent = sendto(fd, buf, (size_t)len, MSG_DONTWAIT, (struct sockaddr *)&addr, sizeof(addr));

    if(sent < 0 && errno != EAGAIN && errno != EWOULDBLOCK && errno != ENOENT) {
        ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "aegis-mod: sendto(%s) failed: %s", cfg->agent_socket, strerror(errno));
    }

    close(fd);
}

int aegis_access_checker(request_rec *r) {
    aegis_dir_cfg *dir = ap_get_module_config(r->per_dir_config, &aegis_module);
    if(!dir || !dir->enabled) return DECLINED;

    aegis_server_cfg *cfg = ap_get_module_config(r->server->module_config, &aegis_module);
    if(!cfg) return DECLINED;

    /* 내부 서브리퀘스트 제외 */
    if(r->main) return DECLINED;

    char buf[AEGIS_SOCKET_BUF_SIZE];
    int len = build_message(r, cfg, buf, sizeof(buf));
    if(len < 0) {
        ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, r, "aegis-mod: payload too large for socket buffer, skipping");
        return DECLINED;
    }

    send_agent(r, cfg, buf, len);

    return DECLINED;
}