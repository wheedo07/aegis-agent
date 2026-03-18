#include "agent.http.h"
#include<sstream>

AegisHttpServer::AegisHttpServer() {
    curl = curl_easy_init();
}

AegisHttpServer::~AegisHttpServer() {
    if(curl) curl_easy_cleanup(curl);
}

AegisHttpResponse AegisHttpServer::handle_request(AegisMessage msg) {
    AegisHttpResponse response;
    if(!curl) return response;

    string url = msg.control_url + AEGIS_HTTP_API_CHECK;
    string body = build_json(msg);
    string response_string;

    curl_easy_reset(curl);
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)body.size());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, AEGIS_HTTP_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);

    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return response;
    }

    Json::Value root;
    Json::CharReaderBuilder builder;
    string errs;

    istringstream ss(response_string);
    if(!Json::parseFromStream(builder, ss, &root, &errs)) {
        fprintf(stderr, "Failed to parse JSON response: %s\n", errs.c_str());
        return response;
    }

    if(root.isMember("action") && root["action"].asString() == "BLOCK") {
        response.block = true;
        response.reason = root.get("reason", "Blocked by control server").asString();
        response.ttl = root.get("ttl", 300).asInt();
    } else {
        response.block = false;
    }

    return response;
}

string AegisHttpServer::build_json(AegisMessage msg) {
    Json::Value root;
    root["ip"] = msg.ip;
    root["method"] = msg.method;
    root["uri"] = msg.uri;
    root["host"] = msg.host;
    root["query"] = msg.query;
    root["user_agent"] = msg.user_agent;
    root["content_type"] = msg.content_type;
    root["referer"] = msg.referer;
    root["x_forwarded_for"] = msg.x_forwarded_for;
    root["protocol"] = msg.protocol;
    root["status_code"] = msg.status_code;
    root["timestamp"] = (Json::Int64)msg.timestamp;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    return Json::writeString(builder, root);
}

size_t AegisHttpServer::write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    static_cast<string *>(userdata)->append(ptr, size * nmemb);
    return size * nmemb;
}