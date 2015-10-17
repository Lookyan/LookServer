#include "httpserver.h"
#include "client.h"
#include <event2/thread.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <sys/time.h>
#include "httpparser.h"
#include "configuration.h"

workqueue_t HttpServer::workqueue;

HttpServer::HttpServer()
{
    evthread_use_pthreads();
    //TODO: Signal handling	
}

HttpServer::~HttpServer()
{
    
}

void HttpServer::startServer(int port, int nCPU, char* docRoot)
{
    if(docRoot != NULL) {
        Configuration::DOCUMENT_ROOT = docRoot;
    }
    struct sockaddr_in listenAddr;
    struct event_base *base;
    struct evconnlistener *listener;
    
    base = event_base_new();
    if (!base) {
            puts("Couldn't open event base");
//            return 1;
    }
    
    if (evthread_make_base_notifiable(base)<0) {
        std::cout << "Couldn't make base notifiable!";
        //return 1;
    }
    
    memset(&listenAddr, 0, sizeof(listenAddr));
    
    listenAddr.sin_family = AF_INET; //for TCP/IP
    /* Listen on 0.0.0.0 */
    listenAddr.sin_addr.s_addr = htonl(0);
    /* Listen on the given port. */
    if(port == 0) {
        port = Configuration::PORT;
    }
    listenAddr.sin_port = htons(port);
    
    /* Initialize work queue. */
    if(nCPU == 0) {
        nCPU = Configuration::numThreads;
    }
    WorkQueue::workqueue_init((workqueue_t *)&workqueue, nCPU);
    
    listener = evconnlistener_new_bind(base, acceptConnCb, (void *)&workqueue,
        LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE|LEV_OPT_THREADSAFE, Configuration::maxConn,
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

void HttpServer::acceptConnCb(evconnlistener *listener, int fd, sockaddr *address, int socklen, void *arg)
{
    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
    
    workqueue_t *workqueue = (workqueue_t *)arg;
    Client* client = new Client();
    client->setBufEv(bev);
    client->setEvBase(base);
    client->setOutputBuffer(evbuffer_new());
   
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
    
    char *request_line = (char *)calloc(1000, sizeof(char));
    size_t len;
    
    evbuffer_copyout(input, request_line, evbuffer_get_length(input));
    HttpParser* httpParser = new HttpParser();
    evbuffer* temp = httpParser->setRequest(request_line);
    
    //std::cout << request_line << std::endl;
    delete httpParser;
    free(request_line);
    
    evbuffer_add_buffer(output, temp);
    
    evbuffer_free(temp);
}

void HttpServer::echoEventCb(bufferevent *bev, short events, void *ctx)
{
    if (events & BEV_EVENT_ERROR)
        std::cout << "bufferevent error!" << std::endl;
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        bufferevent_free(bev);
    }
}

void HttpServer::writeCb(bufferevent *bev, void *ctx)
{
    bufferevent_free(bev);
}

void HttpServer::serverJobFunction(job *job)
{
    Client *client = (Client *)job->user_data;
    //bufferevent_lock(client->getBufEv());    
//    evbuffer_enable_locking(bufferevent_get_input(client->getBufEv()), NULL);
//    evbuffer_enable_locking(bufferevent_get_output(client->getBufEv()), NULL);
    bufferevent_setcb(client->getBufEv(), echoReadCb, writeCb, echoEventCb, NULL);
    bufferevent_enable(client->getBufEv(), EV_PERSIST|EV_TIMEOUT|EV_READ);
    
    delete client;
	//closeAndFreeClient(client);
	delete job;
    //bufferevent_unlock(client->getBufEv());
}
