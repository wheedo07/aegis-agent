#ifndef __AEGIS_AGENT_SOCKET_H__
#define __AEGIS_AGENT_SOCKET_H__
#include<string>
#include<vector>
#include<thread>
#include<mutex>
#include<queue>
#include<atomic>
#include<functional>
#include<condition_variable>
using namespace std;

#define AEGIS_RECV_BUF_SIZE 8192
#define AEGIS_WORKER_COUNT 4

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
    int status_code = 0;
    long timestamp;
    
    bool valid() {
        return !ip.empty() && !uri.empty();
    }
} AegisMessage;

class AegisSocketServer {
    private:
        int fd = -1;
        string socket_path;
        vector<thread> workers;
        queue<string> msg_queue;
        mutex queue_mutex;
        condition_variable cv;
        function<void(AegisMessage msg)> callback;
        atomic<bool> running = false;

        bool parse_message(const char *buf, size_t len, AegisMessage &out);

    public:
        AegisSocketServer(string socket_path);
        ~AegisSocketServer();

        /* 끝나는 시점까지 반환을 대기 */
        bool start();
        
        void stop();
        void loop();
        void on_message(function<void(AegisMessage msg)> callback) {
            this->callback = callback;
        }
};

#endif