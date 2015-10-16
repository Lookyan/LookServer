#include "httpparser.h"
#include "configuration.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

const char* HttpParser::DOCUMENT_ROOT = "/home/alex/ldocroot";
const char* HttpParser::INDEX_FILE = "index.html";

HttpParser::HttpParser()
{
}

evbuffer* HttpParser::setRequest(char *req)
{
    this->req = req;
    
    //parse first line of request:
    char* method;
    method = strtok(req, " ");
    if(method == NULL || (strcmp(method, "GET") != 0 && strcmp(method, "HEAD") != 0)) {
        return (new Response(405))->getRawResponse();
    }
    //std::cout << "Method: " << method << std::endl;
    
    char* file;
    file = strtok(NULL, " ");
    if(file == NULL) {
        return (new Response(400))->getRawResponse();
    }
    //std::cout << "File: " << file << std::endl;    
    
    char* path = (char *)malloc(1000);
    strcpy(path, DOCUMENT_ROOT);
    strcat(path, file);
        
    if(file[strlen(file) - 1] == '/') {
        strcat(path, INDEX_FILE);
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
    
    
    FILE *f;
    f = fopen(path, "rb");
    if(f == NULL) {
        free(path);
        return (new Response(404))->getRawResponse();
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    //fseek(f, 0, SEEK_SET);
    rewind(f);
    
    char *string = (char*)malloc(fsize + 1);
    int result = fread(string, fsize, 1, f);
    fclose(f);
        
    string[fsize] = '\0';
    
    //std::cout << string << std::endl;
    
    Response* response = new Response(200);
    response->addHeader("Content-type", getContentType(extension));
    evbuffer *temp = response->getRawResponse();
    evbuffer_add_printf(temp, "%s", string);
    
    if(string != NULL)
        free(string);
    if(path != NULL)
        free(path);
    if(extension != NULL)
        free(extension);
    return temp;
    
}

const char *HttpParser::getContentType(char *extension)
{
    if(extension == NULL) {
        return Configuration::ext.at("no_ext");
    } else {
        return Configuration::ext[extension];
    }
}
