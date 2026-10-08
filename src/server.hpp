#ifndef SERVER_H
#define SERVER_H

#include <mutex>
#include <string>
#include <unordered_map>

using Store = std::unordered_map<std::string, std::string>;

class Server{
    public:
        Server(int port);
        ~Server();
        void run();
    private:
        Store store;
        std::mutex store_mutex;
        int server_fd = -1;
        int port;

        bool setup_socket();
        void handle_client(int client_fd);
        std::string handle_command(const std::string& input);
};

#endif