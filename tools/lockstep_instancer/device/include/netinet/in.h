#pragma once
// socket networking does not exist on the GPU; types only, for headers
// that declare TCP-backed structs which are never instantiated there
#include <stdint.h>
typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;
struct in_addr { in_addr_t s_addr; };
struct sockaddr_in {
    uint16_t sin_family;
    in_port_t sin_port;
    struct in_addr sin_addr;
    unsigned char sin_zero[8];
};
