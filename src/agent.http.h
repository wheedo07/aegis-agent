#ifndef __AEGIS_AGENT_HTTP_H__
#define __AEGIS_AGENT_HTTP_H__
#include "agent.socket.h"
#include<curl/curl.h>
#include<json/json.h>
#include<string>
using namespace std;

#define AEGIS_HTTP_API_CHECK "/api/agent/check"
#define AEGIS_HTTP_TIMEOUT_MS 2000L

typedef struct {
    bool block = false;
    string reason;
} AegisHttpResponse;

class AegisHttpServer {
    private:
        CURL *curl;

        static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata);
        static string build_json(AegisMessage msg);
        
    public:
        AegisHttpServer();
        ~AegisHttpServer();

        AegisHttpResponse handle_request(AegisMessage msg);
};

#endif