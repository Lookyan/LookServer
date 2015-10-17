#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include "response.h"

class HttpParser
{
public:
    HttpParser();
    evbuffer *setRequest(char* req);
    
    /*
     * @return free char* outside this class 
     * */
    
private:
    const char* getContentType(char* extension);
    void urldecode(char *pszDecodedOut, size_t nBufferSize, char *pszEncodedIn);
    
    char* req;
    char* path;
    static const char* DOCUMENT_ROOT;
    static const char* INDEX_FILE;
};

#endif // HTTPPARSER_H
