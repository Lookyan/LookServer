#include "client.h"

Client::Client()
{
}

Client::~Client()
{    
    if(output_buffer != NULL) {
        evbuffer_free(output_buffer);
    }
}
