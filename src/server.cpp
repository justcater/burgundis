#include "server.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

Server::Server(int port): port(port) {
    replay_log();
    log.open("data.log", std::ios::app);
    if (!log.is_open()) {
        std::cerr << "open() failed\n";
    }
}

Server::~Server(){
    if (server_fd >= 0) {
        close(server_fd);
    }
    if (log.is_open()) {
        log.close();
    }
}

bool Server::log_command(const std::string& input) {
    log << input << '\n';
    log.flush();
    if (!log) {
        return false;
    }
    return true;
}

void Server::replay_log() {
    std::ifstream in("data.log");
    std::string line;

    if (!in.is_open()) return;

    while(std::getline(in, line)) {
        if (line.empty()) continue;
        handle_command(line, false);
    }

    if (!in.eof()) {
        std::cerr << "Warning: reading from log failed.\n";
    }
}

std::string Server::handle_command(const std::string& input, bool logging) {
    std::istringstream iss(input);
    std::string cmd, key, value;
    iss >> cmd >> key >> value;

    if ((cmd == "SET" || cmd == "DEL") && logging == true) {
        if(!log_command(input)) {
            return "ERR writing to log failed\n";
        }
    }

    if (cmd == "SET") {
        store[key] = value;
        return "OK\n";
    }
    else if (cmd == "GET") {
        auto it = store.find(key);
        return (it != store.end() ? it->second + "\n" : "(nil)\n");
    }
    else if (cmd == "DEL") {
        store.erase(key);
        return "OK\n";
    }
    else return "ERR command unknown\n";
}

void Server::handle_client(int client_fd){
    char buffer[1024];
    std::string pending;

    while (true) {
        ssize_t n = read(client_fd, buffer, sizeof(buffer)-1);
        if (n <= 0) break;

        pending.append(buffer, n);

        size_t pos;
        while ((pos = pending.find('\n')) != std::string::npos) {
            std::string line = pending.substr(0, pos);
            pending.erase(0, pos + 1);
            if (line.empty()) continue;

            std::lock_guard<std::mutex> lock(store_mutex);
            std::string reply = handle_command(line, true);
            write(client_fd, reply.c_str(), reply.size());
        }
    }

    close(client_fd);
}

bool Server::setup_socket(){
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "socked() failed\n";
        return false;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt() failed\n";
        close(server_fd);
        server_fd = -1;
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind() failed\n";
        close(server_fd);
        server_fd = -1;
        return false;
    }

    if (listen(server_fd, 5) < 0) {
        std::cerr << "listen() failed\n";
        close(server_fd);
        server_fd = -1;
        return false;
    }

    std::cout << "Listening on port " << port << '\n';
    return true;
}

void Server::run(){
    if (!setup_socket()) return;
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            std::cerr << "accept() failed\n";
            continue;
        }
        std::thread(&Server::handle_client, this, client_fd).detach();
    }
}
