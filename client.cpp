#include "client.h"

Client::Client()
{
}

Client::~Client()
{
//    if(evbase != NULL) {
//        event_base_free(evbase);
//    }
    
//    if(buf_ev != NULL) {
//        bufferevent_free(buf_ev);
//    }
    
    if(output_buffer != NULL) {
        evbuffer_free(output_buffer);
    }
}
