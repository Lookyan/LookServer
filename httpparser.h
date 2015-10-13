#ifndef HTTPPARSER_H
#define HTTPPARSER_H

class HttpParser
{
public:
    HttpParser();
    void setRequest(char* req);
    
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
