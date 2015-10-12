#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <arpa/inet.h>
#include "workqueue.h"

class HttpServer
{
public:
    HttpServer();
    virtual ~HttpServer();
    void startServer();
    
private:
    static void acceptConnCb(struct evconnlistener *listener,
        evutil_socket_t fd, struct sockaddr *address, int socklen,
        void *ctx);
    static void acceptErrorCb(struct evconnlistener *listener, void *ctx);
    static void echoReadCb(struct bufferevent *bev, void *ctx);
    static void echoEventCb(struct bufferevent *bev, short events, void *ctx);
    static void serverJobFunction(struct job *job);
    static void writeCb(bufferevent *bev, void *ctx);
    
    const static int port = 5555;
    const static int maxConn = 8;
    const static int numThreads = 8;
    static workqueue_t workqueue;
};

#endif // HTTPSERVER_H
