#include "configuration.h"

std::map<std::string, const char*> Configuration::ext = {
    {"no_ext", "text/plain"},
    {"html", "text/html"},
    {"css", "text/css"},
    {"js", "text/javascript"},
    {"jpg", "image/jpeg"},
    {"jpeg", "image/jpeg"},
    {"png", "image/png"},
    {"gif", "image/gif"},
    {"swf", "application/x-shockwave-flash"}
};

Configuration::Configuration()
{
}
