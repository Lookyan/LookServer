#include "httpserver.h"
#include "client.h"
#include <iostream>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <sys/time.h>

workqueue_t HttpServer::workqueue;

HttpServer::HttpServer()
{
    //TODO: Signal handling
    
	struct sockaddr_in listenAddr;
    struct event_base *base;
    struct evconnlistener *listener;
    
    base = event_base_new();
    if (!base) {
            puts("Couldn't open event base");
//            return 1;
    }
    
    memset(&listenAddr, 0, sizeof(listenAddr));
    
    listenAddr.sin_family = AF_INET; //for TCP/IP
    /* Listen on 0.0.0.0 */
    listenAddr.sin_addr.s_addr = htonl(0);
    /* Listen on the given port. */
    listenAddr.sin_port = htons(port);
    
    /* Initialize work queue. */
    WorkQueue::workqueue_init((workqueue_t *)&workqueue, numThreads);
    
    listener = evconnlistener_new_bind(base, acceptConnCb, (void *)&workqueue,
        LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE|LEV_OPT_THREADSAFE, maxConn,
        (struct sockaddr*)&listenAddr, sizeof(listenAddr));
    if (!listener) {
//            perror("Couldn't create listener");
//            return 1;
    }
    
    evconnlistener_set_error_cb(listener, acceptErrorCb);

    event_base_dispatch(base);
    
    event_base_free(base);
    WorkQueue::workqueue_shutdown(&workqueue);
}

HttpServer::~HttpServer()
{
    
}

void HttpServer::startServer()
{
    
}

void HttpServer::acceptConnCb(evconnlistener *listener, int fd, sockaddr *address, int socklen, void *arg)
{
    std::cout << "yes" << std::endl;
    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    
    workqueue_t *workqueue = (workqueue_t *)arg;
    Client* client = new Client();
    client->setBufEv(bev);
    client->setEvBase(base);
    client->setOutputBuffer(evbuffer_new());
    timeval* readTimeout = new timeval();
    readTimeout->tv_sec = 2;
    timeval* writeTimeout = new timeval();
    writeTimeout->tv_sec = 2;
    //bufferevent_set_timeouts(bev, readTimeout, writeTimeout); // TODO: constants remove
    bufferevent_enable(client->getBufEv(), EV_READ); 
    job_t *job = new job_t;
    job->job_function = serverJobFunction;
	job->user_data = client;
    WorkQueue::workqueue_add_job(workqueue, job);
}

void HttpServer::acceptErrorCb(evconnlistener *listener, void *ctx)
{
    struct event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));

    event_base_loopexit(base, NULL);
}

void HttpServer::echoReadCb(bufferevent *bev, void *ctx)
{
    struct evbuffer *input = bufferevent_get_input(bev);
    struct evbuffer *output = bufferevent_get_output(bev);
    
//    char *request_line;
//    size_t len;
    
//    do {
//        request_line = evbuffer_readln(input, &len, EVBUFFER_EOL_CRLF);
//        std::cout << request_line << std::endl;
//    } while(request_line != NULL);

    /* Copy all the data from the input buffer to the output buffer. */
    evbuffer_add_buffer(output, input);
}

void HttpServer::echoEventCb(bufferevent *bev, short events, void *ctx)
{
    if (events & BEV_EVENT_ERROR)
            perror("Error from bufferevent");
//    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
//            bufferevent_free(bev);
//    }
}

void HttpServer::writeCb(bufferevent *bev, void *ctx)
{
    std::cout << "write" << std::endl;
    bufferevent_free(bev);
}

void HttpServer::serverJobFunction(job *job)
{
    Client *client = (Client *)job->user_data;
    bufferevent_setcb(client->getBufEv(), echoReadCb, writeCb, echoEventCb, NULL);

    delete client;
	//closeAndFreeClient(client);
	free(job);
}