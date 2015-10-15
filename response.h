#ifndef RESPONSE_H
#define RESPONSE_H

#include <event2/buffer.h>

class Response
{
public:
    Response(int code);
    ~Response();
    void addHeader(char* name, char* value);
    evbuffer* getRawResponse();
    
private:
    const char* getNameByCode(int code);
    evbuffer* response;
};

#endif // RESPONSE_H
