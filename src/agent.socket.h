#ifndef __AEGIS_AGENT_SOCKET_H__
#define __AEGIS_AGENT_SOCKET_H__
#include<string>
#include<functional>
using namespace std;

typedef struct {
    string version;
    string ip;
    string method;
    string uri;
    string host;
    string query;
    string user_agent;
    string content_type;
    string content_length;
    string referer;
    string x_forwarded_for;
    string protocol;
    string control_url;
    long timestamp;
    
    bool valid() {
        return !ip.empty() && !uri.empty() && !control_url.empty();
    }
} AegisMessage;

class AegisSocketServer {
    private:
        int fd = -1;
        string socket_path;
        function<void(AegisMessage msg)> callback;

        bool parse_message(char *buf, size_t len, AegisMessage &out);

    public:
        AegisSocketServer(string socket_path);
        ~AegisSocketServer();

        void _process();
        void on_message(function<void(AegisMessage msg)> callback) {
            this->callback = callback;
        }
};

#endif