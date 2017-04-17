#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/queue.h>
#include <unistd.h>
#include <string.h>

#ifndef CONTROL_HANDLER_LIB_H_
#define CONTROL_HANDLER_LIB_H_

#define CNTRL_HEADER_SIZE 8
#define CNTRL_RESP_HEADER_SIZE 8
#define CNTRL_PAYLOAD_SIZE 8
#define PACKET_USING_STRUCT // Comment this out to use alternate packet crafting technique

uint16_t update_interval;
uint16_t router_number;
//Router udp
int sock[5];
fd_set master_list, watch_list;
int head_fd;
char router_id;


struct sockaddr_in update_addr[5];

struct CONTROL_PAYLOAD
    {
        uint32_t router_ip_addr;
        uint16_t router_id;
	uint16_t router_port1;
	uint16_t router_port2;
	uint16_t cost;
	
    }payload[5], *payload_temp;
char* create_response_header(int sock_index, uint8_t control_code, uint8_t response_code, uint16_t payload_len);

struct CONTROL_RESPONSE_PAYLOAD
    {
        uint16_t router_id;
        uint16_t padding;
	uint16_t router_nexthop;
	uint16_t router_cost;
    }response_payload[5];

struct init_packet{
uint16_t cost;
uint16_t router_id;
uint16_t nexthop_id;
uint16_t padding;
}initres[5];

struct router_update{
uint16_t cost;
uint16_t router_id;
uint16_t nexthop_id;
uint16_t padding;
}router[5];

struct table{
uint16_t cost;
uint16_t next_hop;
uint16_t id;
uint16_t padding;
}tab[10][10];
#endif

