#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include "response.h"

class HttpParser
{
public:
    HttpParser();
    evbuffer *setRequest(char* req);
    
private:
    const char* getContentType(char* extension);
    void urldecode(char *pszDecodedOut, size_t nBufferSize, char *pszEncodedIn);
};

#endif // HTTPPARSER_H
