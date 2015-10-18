#include "response.h"
#include <time.h>


Response::Response(int code)
{
    response = evbuffer_new();
    evbuffer_add_printf(response, "HTTP/1.1 %d %s\r\n", code, getNameByCode(code));
    addHeader("Server", "LookServer/0.1");
    
    //form date value
    time_t rawtime;
    struct tm * ptm;
    time (&rawtime);
    ptm = gmtime (&rawtime);
    char date[80];
    strftime (date, 80, "%a, %d %b %Y %H:%M:%S %Z", ptm);
    addHeader("Date", date);
    addHeader("Connection", "Close");
}

Response::~Response()
{
    
}

void Response::addHeader(const char *name, const char *value)
{
    evbuffer_add_printf(response, "%s: %s\r\n", name, value);
}

void Response::addHeader(const char *name, int value) 
{
    evbuffer_add_printf(response, "%s: %d\r\n", name, value);
}

evbuffer *Response::getRawResponse()
{
    evbuffer_add_printf(response, "\r\n");
    return response;
}

const char *Response::getNameByCode(int code)
{
    switch(code) {
    case 200:
        return "OK";
        break;
    case 403:
        return "Forbidden";
        break;
    case 404:
        return "Not Found";
        break;
    case 405:
        return "Method Not Allowed";
        break;
    case 400:
        return "Bad Request";
        break;
    }
}

