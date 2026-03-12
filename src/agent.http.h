#ifndef __AEGIS_AGENT_HTTP_H__
#define __AEGIS_AGENT_HTTP_H__
#include "agent.socket.h"
#include<curl/curl.h>
#include<json/json.h>
#include<string>
using namespace std;

typedef struct {
    bool block = false;
    string reason;
} AegisHttpResponse;

class AegisHttpServer {
    private:
        CURL *curl;

        string build_json(AegisMessage msg);
        size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata);
        
    public:
        AegisHttpServer();
        ~AegisHttpServer();

        AegisHttpResponse handle_request(AegisMessage msg);
};

#endif