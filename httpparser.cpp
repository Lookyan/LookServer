#include "httpparser.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

const char* HttpParser::DOCUMENT_ROOT = "/home/alex/ldocroot";

HttpParser::HttpParser()
{
}

void HttpParser::setRequest(char *req)
{
//    this->req = req;
    
//    //parse first line of request:
//    char* method;
//    method = strtok(req, " ");
//    std::cout << "Method: " << method << std::endl;
    
//    char* file;
//    file = strtok(NULL, " ");
//    std::cout << "File: " << file << std::endl;
    
    
//    if(strcmp(file, "/") == 0) {
//        file = "/index.html";
//        std::cout << "index.html" << std::endl;
//    }
//    char* path = (char *)malloc(1000);
//    strcpy(path, DOCUMENT_ROOT);
//    strcat(path, file);
    
//    FILE *f;
//    f = fopen(path, "r");
//    if(f == NULL) {
//        return;
//    }
//    fseek(f, 0, SEEK_END);
//    long fsize = ftell(f);
//    fseek(f, 0, SEEK_SET);
    
//    char *string = (char*)malloc(fsize + 1);
//    fread(string, fsize, 1, f);
//    fclose(f);
        
//    string[fsize] = '\0';
    
//    std::cout << string << std::endl;
    
//    free(string);
//    free(path);
    
}

char *HttpParser::getResponse()
{
    
}

char *HttpParser::getFileContent()
{
    
}
