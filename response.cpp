#include "response.h"
#include <time.h>

/*
 * free response after return 
*/
Response::Response(int code)
{
    response = evbuffer_new();
    evbuffer_add_printf(response, "HTTP/1.1 %d ", code);
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
    evbuffer_add_printf(response, "\r\n\r\n");
    
//    switch (code) {
//    case 405:
        
//        break;
//    default:
//        break;
//    }
}

Response::~Response()
{
    
}

void Response::addHeader(char *name, char *value)
{
    evbuffer_add_printf(response, "%s: %s\r\n", name, value);
}

evbuffer *Response::getRawResponse()
{
    return response;
}

const char *Response::getNameByCode(int code)
{
    switch(code) {
    case 200:
        return "OK";
        break;
    case 404:
        return "Not Found";
        break;
    case 405:
        return "Method Not Allowed";
        break;
    }
}

