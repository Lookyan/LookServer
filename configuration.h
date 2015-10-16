#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <map>
#include <string>

class Configuration
{
public:
    static std::map<std::string, const char*> ext;
//    static const char* NO_EXT = "text/plain";
//    static const char* HTML_EXT = "text/html";
//    static const char* CSS_EXT = "text/css";
//    static const char* JS_EXT = "text/javascript";
//    static const char* JPG_EXT = "image/jpeg";
//    static const char* JPEG_EXT = "image/jpeg";
//    static const char* PNG_EXT = "image/png";
//    static const char* GIF_EXT = "image/gif";
//    static const char* SWF_EXT = "application/x-shockwave-flash";
private:
    Configuration();    
};

#endif // CONFIGURATION_H
