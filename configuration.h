#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <map>
#include <string>

class Configuration
{
public:
    static std::map<std::string, const char*> ext;
    static const int PORT = 8080;
    static const int maxConn = 20;
    static const int numThreads = 4;
    static char* DOCUMENT_ROOT;
    static char* INDEX_FILE;
private:
    Configuration();    
};

#endif // CONFIGURATION_H
