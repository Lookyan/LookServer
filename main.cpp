#include "httpserver.h"
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <exception>

int main(int argc, char **argv)
{    
    int port = 0;
    int nCPU = 0;
    char* rootDir = 0;
    
    int c = 0;
    while ((c = getopt (argc, argv, "p:n:r:")) != -1) {
        switch(c) {
        case 'p':
            port = atoi(optarg);
            if(port < 1 || port > 65535) {
                std::cout << "Wrong port: " << port << std::endl;
                return 0;
            }
            break;
        case 'n':
            nCPU = atoi(optarg);
            if(nCPU < 1) {
                std::cout << "Wrong nCPU: " << nCPU << std::endl;
                return 0;
            }
            break;
        case 'r':
            rootDir = optarg;
            if(rootDir[strlen(rootDir) - 1] == '/') {
                rootDir[strlen(rootDir) - 1] = '\0';
            }
            break;
        }
    }
    
    HttpServer* httpServer = 0;
    try {
        HttpServer* httpServer = new HttpServer();
        httpServer->startServer(port, nCPU, rootDir);
    }
    catch(std::exception& err) {
        std::cout << err.what() << std::endl;
        delete httpServer;
    }
    
    delete httpServer;
    return 0;
}
