#ifndef __AEGIS_AGENT_HTTP_H__
#define __AEGIS_AGENT_HTTP_H__
#include<curl/curl.h>
#include<json/json.h>
#include<string>
#define AEGIS_HTTP_TIMEOUT_MS 2000L

class AegisHttpServer {
    private:
        CURL *curl;
        
        static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata);
        
    public:
        AegisHttpServer();
        ~AegisHttpServer();

        bool post(std::string url, Json::Value body, Json::Value &response_json);
};

#endif