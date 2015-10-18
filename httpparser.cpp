#include "httpparser.h"
#include "configuration.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

HttpParser::HttpParser()
{
}

evbuffer* HttpParser::setRequest(char *req)
{    
    //parse first line of request:
    char* method;
    method = strtok(req, " ");
    if(method == NULL || (strcmp(method, "GET") != 0 && strcmp(method, "HEAD") != 0)) {
        return (new Response(405))->getRawResponse();
    }
    
    char* file;
    file = strtok(NULL, " ");
    if(file == NULL) {
        return (new Response(400))->getRawResponse();
    }
    
    char* decoded = (char*) calloc(1000, sizeof(char));
    urldecode(decoded, 1000, file);
    file = decoded;
    
    //remove GET params
    char* index = strrchr(file, '?');
    if(index != NULL) {
        index[0] = '\0';
    }
    
    char* path = (char *)malloc(1000);
    strcpy(path, Configuration::DOCUMENT_ROOT);
    strcat(path, file);
        
    bool isindex = false;
    if(file[strlen(file) - 1] == '/') {
        strcat(path, Configuration::INDEX_FILE);
        isindex = true;
    }
    
    //check extension
    char* extension = 0;
    int i = strlen(path) - 1;
    while(path[i] != '/') {
        --i;
        if(path[i] == '.') {
            extension = (char*) calloc(strlen(path) - i, sizeof(char));
            strcpy(extension, path + i + 1);
            break;
        }
    }        
    
    
    if(strstr(path, "/../") != NULL) {
        free(path);
        free(decoded);
        return (new Response(404))->getRawResponse();
    }
    
    int fd = open (path, O_RDONLY);
    if(fd < 0) {
        free(path);
        free(decoded);
        if(isindex) {
            return (new Response(403))->getRawResponse();
        } else {
            return (new Response(404))->getRawResponse();
        }
    }
    struct stat stat_;
    fstat (fd, &stat_);
    
    Response* response = new Response(200);
    response->addHeader("Content-type", getContentType(extension));
    response->addHeader("Content-Length", stat_.st_size);
    evbuffer *temp = response->getRawResponse();
    if(strcmp(method, "GET") == 0) {
        evbuffer_add_file(temp, fd, 0, stat_.st_size);
    }
    
    if(path != NULL)
        free(path);
    if(extension != NULL)
        free(extension);
    free(decoded);
    return temp;
    
}

const char *HttpParser::getContentType(char *extension)
{
    if(extension == NULL) {
        return Configuration::ext["no_ext"];
    } else {
        const char* ext = Configuration::ext[extension];
        if(ext == NULL) {
            return Configuration::ext["default"];
        } else {
            return ext;
        }
    }
}

void HttpParser::urldecode(char *pszDecodedOut, size_t nBufferSize, char *pszEncodedIn)
{
    memset(pszDecodedOut, 0, nBufferSize);

    enum DecodeState_e
    {
        STATE_SEARCH = 0, ///< searching for an ampersand to convert
        STATE_CONVERTING, ///< convert the two proceeding characters from hex
    };

    DecodeState_e state = STATE_SEARCH;

    for(unsigned int i = 0; i <= strlen(pszEncodedIn)-1; ++i)
    {
        if(pszEncodedIn[i] == '+') {
            pszEncodedIn[i] = ' ';
        }
        switch(state)
        {
        case STATE_SEARCH:
            {
                if(pszEncodedIn[i] != '%')
                {
                    strncat(pszDecodedOut, &pszEncodedIn[i], 1);
                    assert(strlen(pszDecodedOut) < nBufferSize);
                    break;
                }

                // We are now converting
                state = STATE_CONVERTING;
            }
            break;

        case STATE_CONVERTING:
            {
                // Conversion complete (i.e. don't convert again next iter)
                state = STATE_SEARCH;

                // Create a buffer to hold the hex. For example, if %20, this
                // buffer would hold 20 (in ASCII)
                char pszTempNumBuf[3] = {0};
                strncpy(pszTempNumBuf, &pszEncodedIn[i], 2);

                // Ensure both characters are hexadecimal
                bool bBothDigits = true;

                for(int j = 0; j < 2; ++j)
                {
                    if(!isxdigit(pszTempNumBuf[j]))
                        bBothDigits = false;
                }

                if(!bBothDigits)
                    break;

                // Convert two hexadecimal characters into one character
                int nAsciiCharacter;
                sscanf(pszTempNumBuf, "%x", &nAsciiCharacter);

                // Ensure we aren't going to overflow
                assert(strlen(pszDecodedOut) < nBufferSize);

                // Concatenate this character onto the output
                strncat(pszDecodedOut, (char*)&nAsciiCharacter, 1);

                // Skip the next character
                i++;
            }
            break;
        }
    }
}
