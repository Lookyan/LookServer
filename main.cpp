#include "httpserver.h"

int main(int argc, char **argv)
{    
    HttpServer* httpServer = new HttpServer();
    httpServer->startServer();
    return 0;
}
