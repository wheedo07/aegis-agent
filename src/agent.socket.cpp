#include "agent.socket.h"
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sstream>
#include<cstring>
#include<iostream>

AegisSocketServer::AegisSocketServer(string socket_path) {
    this->socket_path = socket_path;
}

AegisSocketServer::~AegisSocketServer() {
    stop();
}

bool AegisSocketServer::start() {
    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(fd < 0) {
        cerr << "[aegis-agent] socket() failed: " << strerror(errno) << endl;
        return false;
    }
    ::unlink(socket_path.c_str());

    struct sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path.c_str(), sizeof(addr.sun_path) - 1);

    if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "[aegis-agent] bind(" << socket_path << ") failed: " << strerror(errno) << endl;
        ::close(fd);
        fd = -1;
        return false;
    }
    
    chmod(socket_path.c_str(), 0666);

    running = true;

    for(int i=0; i < AEGIS_WORKER_COUNT; i++) {
        workers.emplace_back(&AegisSocketServer::loop, this);
    }
    cout << "[aegis-agent] listening on " << socket_path << endl;

    char buf[AEGIS_RECV_BUF_SIZE];
    while(running.load()) {
        ssize_t len = recvfrom(fd, buf, sizeof(buf) - 1, 0, nullptr, nullptr);
        if(len < 0) {
            if(errno == EINTR) continue;
            cerr << "[aegis-agent] recvfrom() failed: " << strerror(errno) << endl;
            break;
        }
        buf[len] = '\0';

        {
            lock_guard<mutex> lock(queue_mutex);
            msg_queue.emplace(buf, len);
        }
        cv.notify_one();
    }

    return true;
}

void AegisSocketServer::stop() {
    if(!running.exchange(false)) return;
    cv.notify_all();

    for(auto &t : workers) {
        if(t.joinable()) t.join();
    }
    workers.clear();

    if(fd >= 0) {
        ::close(fd);
        fd = -1;
        ::unlink(socket_path.c_str());
    }
    printf("[aegis-agent] stopped\n");
}

void AegisSocketServer::loop() {
    while(true) {
        string raw;
        {
            unique_lock<mutex> lock(queue_mutex);
            cv.wait(lock, [this]() {
                return !msg_queue.empty() || !running.load();
            });

            if(!running.load() && msg_queue.empty()) break;

            raw = move(msg_queue.front());
            msg_queue.pop();
        }

        {
            AegisMessage msg;
            if(!parse_message(raw.c_str(), raw.size(), msg)) continue;
            if(callback) callback(msg);
        }
    }
}

bool AegisSocketServer::parse_message(const char *buf, size_t len, AegisMessage &out) {
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
        }else if(key == "status_code") {
            try {
                out.status_code = stoi(value);
            } catch(...) {
                out.status_code = 0;
            }
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