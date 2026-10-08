#ifndef SERVER_H
#define SERVER_H

#include <mutex>
#include <string>
#include <unordered_map>
#include <fstream>

using Store = std::unordered_map<std::string, std::string>;

class Server{
    public:
        Server(int port);
        ~Server();
        void run();
    private:
        Store store;
        std::mutex store_mutex;
        std::ofstream log;
        int server_fd = -1;
        int port;

        bool setup_socket();
        bool log_command(const std::string& input);
        void replay_log();
        void handle_client(int client_fd);
        std::string handle_command(const std::string& input, bool logging);
};

#endif