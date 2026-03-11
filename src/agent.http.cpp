#include "agent.http.h"

AegisHttpServer::AegisHttpServer() {
    curl = curl_easy_init();
}

AegisHttpServer::~AegisHttpServer() {
    if(curl) curl_easy_cleanup(curl);
}

AegisHttpResponse AegisHttpServer::handle_request(AegisMessage msg) {
}