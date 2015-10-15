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
    char* getResponse();
    
private:
    char* getFileContent();
    char* req;
    char* path;
    static const char* DOCUMENT_ROOT;
};

#endif // HTTPPARSER_H
