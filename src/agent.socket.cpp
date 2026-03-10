#include "agent.socket.h"
#include<sstream>

AegisSocketServer::AegisSocketServer(string socket_path) {
    this->socket_path = socket_path;
}

AegisSocketServer::~AegisSocketServer() {}

void AegisSocketServer::_process() {
}

bool AegisSocketServer::parse_message(char *buf, size_t len, AegisMessage &out) {
    if(!buf || len == 0) return false;

    istringstream ss(string(buf, len));
    string line;
    bool is = false;

    while(getline(ss, line)) {
        if(!line.empty() && line.back() == '\r') line.pop_back();
        if(line.empty()) {
            is = true;
            break;
        }

        auto pos = line.find('=');
        if(pos == string::npos) continue;

        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);

        if(key == "version") {
            out.version = value;
        }else if(key == "ip") {
            out.ip = value;
        }else if(key == "method") {
            out.method = value;
        }else if(key == "uri") {
            out.uri = value;
        }else if(key == "host") {
            out.host = value;
        }else if(key == "query") {
            out.query = value;
        }else if(key == "user_agent") {
            out.user_agent = value;
        }else if(key == "content_type") {
            out.content_type = value;
        }else if(key == "content_length") {
            out.content_length = value;
        }else if(key == "referer") {
            out.referer = value;
        }else if(key == "x_forwarded_for") {
            out.x_forwarded_for = value;
        }else if(key == "protocol") {
            out.protocol = value;
        }else if(key == "control_url") {
            out.control_url = value;
        }else if(key == "timestamp") {
            try {
                out.timestamp = stol(value);
            } catch(...) {
                out.timestamp = 0;
            }
        }
    }

    return is && out.valid();
}