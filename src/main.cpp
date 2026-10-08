#include "server.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    int port = 6379;
    
    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        }
        catch (const std::exception&) {
            std::cerr << "Invalid port: " << argv[1] << '\n';
            return 1;
        }

        if (port < 1 || port > 65535) {
            std::cerr << "Port out of range: " << port << '\n';
            return 1;
        }
    }

    Server server(port);
    server.run();
    return 0;
}


