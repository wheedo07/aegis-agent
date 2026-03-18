#ifndef __AEGIS_AGENT_HTTP_H__
#define __AEGIS_AGENT_HTTP_H__
#include "agent.socket.h"
#include<curl/curl.h>
#include<json/json.h>
#include<string>
using namespace std;

#define AEGIS_HTTP_TIMEOUT_MS 2000L
typedef struct {
    bool block = false;
    string reason;
    int ttl = 300; // 차단 유지 시간(초)
} AegisHttpResponse;

class AegisHttpServer {
    private:
        CURL *curl;

        static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata);
        
    public:
        AegisHttpServer();
        ~AegisHttpServer();

        bool post(string url, string body, Json::Value &response_json);
        string message_build(AegisMessage msg);
};

#endif