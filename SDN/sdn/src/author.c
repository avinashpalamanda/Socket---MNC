/**
 * @apalaman_assignment3
 * @author  Avinash Palamanda <apalaman@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */

/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */
#include <string.h>
#include<stdio.h>
#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h"
#include "../include/control_header_lib.h"
#define AUTHOR_STATEMENT "I, apalaman, have read and understood the course academic integrity policy."

void author_response(int sock_index)
{
	uint16_t payload_len, response_len;
	char *cntrl_response_header, *cntrl_response_payload, *cntrl_response;
        printf("%s",AUTHOR_STATEMENT);

	payload_len = sizeof(AUTHOR_STATEMENT)-1; // Discount the NULL chararcter
	cntrl_response_payload = (char *) malloc(payload_len);
	memcpy(cntrl_response_payload, AUTHOR_STATEMENT, payload_len);

	cntrl_response_header = create_response_header(sock_index, 0, 0, payload_len);

	response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
	cntrl_response = (char *) malloc(response_len);
	/* Copy Header */
	memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
	free(cntrl_response_header);
	/* Copy Payload */
	memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, payload_len);
	free(cntrl_response_payload);

	sendALL(sock_index, cntrl_response, response_len);

	free(cntrl_response);
}

void init_response(int sock_index,char *cntrl_payload)
{
	uint16_t response_len,payload_len;
	char *cntrl_response_header,*cntrl_response_payload, *cntrl_response;
	
	//Payload
	memcpy(&router_number, cntrl_payload, sizeof(router_number));
	memcpy(&update_interval, cntrl_payload+0x02, sizeof(update_interval));
	router_number = ntohs(router_number);
	update_interval=ntohs(update_interval);
	//printf("\nnumber of routers %d",update_interval);
	fflush(stdout);
	int i=0;
	int add = 0x00;	
	
	for(i=0;i<router_number;i++){
	memcpy(&payload[i].router_id, cntrl_payload+0x04+add, sizeof(payload[i].router_id));
	memcpy(&payload[i].router_port1, cntrl_payload+add+0x06, sizeof(payload[i].router_port1));
	memcpy(&payload[i].router_port2, cntrl_payload+add+0x08, sizeof(payload[i].router_port2));
	memcpy(&payload[i].cost, cntrl_payload+0x0a+add, sizeof(payload[0].cost));
	memcpy(&payload[i].router_ip_addr, cntrl_payload+0x0c+add, sizeof(payload[i].router_ip_addr));
	add = add + 0x0c;
	//printf("add \n %d",add);
	payload[i].router_id = ntohs(payload[i].router_id);
	payload[i].router_port1 = ntohs(payload[i].router_port1);
	payload[i].router_port2 = ntohs(payload[i].router_port2);
	payload[i].cost = ntohs(payload[i].cost);
	fflush(stdout);
	}

	//Creating UDP Sockets	
	int j =0;
	for (i=0;i<5;i++){
	sock[j]=create_update_sock(payload[i].router_port1,payload[i].cost,i);
	printf("\nUDP%d",sock[j]);
	FD_SET(sock[j], &master_list);
        head_fd = sock[j];	
	fflush(stdout);
	j++;}
	
	//Intiating table
	for(i=0;i<5;i++){
	//Router_ID
	if(payload[i].cost > 0){
	router_id=payload[0].router_id;
	initres[i].router_id=payload[0].router_id;}
	else	
	initres[i].router_id=payload[i].router_id;
	//Next Hop
	if(payload[i].cost!=65535)
	initres[i].nexthop_id=payload[i].router_id;
	//Cost
	initres[i].cost=payload[i].cost;
	printf("\n%d %d %d ",initres[i].router_id,initres[i].nexthop_id,initres[i].cost);
	fflush(stdout);	
	}

	//Matrix;
	for(j=0;j<5;j++){
		if(initres[j].cost == 0){
		for (i=0;i<=5;i++){
		tab[j][i+1].cost = initres[i].cost;
		tab[j][i+1].next_hop= initres[i].nexthop_id;
		tab[j][0].id = initres[i].router_id;
		//printf("\n%d",initres[i].cost);
		printf("\n %d",tab[j][i].cost);
		}}
		else{
		for (i=0;i<5;i++){
		tab[j][i+1].cost = 65535;
		tab[j][i+1].next_hop= 65535;
		tab[j][0].id = initres[i].router_id;
		printf(" %d",tab[j][i+1].cost);
		}}	
		printf("\n");
		fflush(stdout);
	}
	
	//Create Payload
	cntrl_response_payload = (char *) malloc(8);
	//Payload
	for(i=0;i<5;i++){
	int add = 0x00;
	memcpy(cntrl_response_payload+add,&initres[i].router_id, sizeof(payload[0].cost));
	memcpy(cntrl_response_payload+add,&initres[i].padding, sizeof(payload[0].cost));
	memcpy(cntrl_response_payload+add,&initres[i].nexthop_id, sizeof(payload[0].cost));
	memcpy(cntrl_response_payload+add,&initres[i].cost, sizeof(payload[0].cost));
	add =add +0x08;
	}
	payload_len=sizeof(cntrl_response_payload);
	
	//Create header
	cntrl_response_header = create_response_header(sock_index, 1, 1, payload_len);
	response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
	
	//Create Response Packet
	cntrl_response = (char *) malloc(response_len);	
	/* Copy Header */
	memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
	free(cntrl_response_header);
	
	memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload,8);
	free(cntrl_response_payload);
	printf("%s",cntrl_response);
	fflush(stdout);
	
	
	sendALL(sock_index, cntrl_response, response_len);
	for(i=0;i<5;i++){
	sendUDP(sock[i],cntrl_response,response_len,i);
	}
	
	free(cntrl_response);

}

void table_response(int sock_index){
uint16_t response_len,payload_len;
	int i;
	char *cntrl_response_header,*cntrl_response_payload, *cntrl_response;
//Create Payload
	cntrl_response_payload = (char *) malloc(8);
	//Payload
	for(i=0;i<5;i++){
	int add = 0x00;
	memcpy(cntrl_response_payload+add,&initres[i].router_id, sizeof(payload[0].cost));
	memcpy(cntrl_response_payload+add,&initres[i].padding, sizeof(payload[0].cost));
	memcpy(cntrl_response_payload+add,&initres[i].nexthop_id, sizeof(payload[0].cost));
	memcpy(cntrl_response_payload+add,&initres[i].cost, sizeof(payload[0].cost));
	add =add+0x08;
	}
	payload_len=sizeof(cntrl_response_payload);
	
	//Create header
	cntrl_response_header = create_response_header(sock_index, 2, 2, payload_len);
	response_len = CNTRL_RESP_HEADER_SIZE+payload_len;
	
	//Create Response Packet
	cntrl_response = (char *) malloc(response_len);	
	/* Copy Header */
	memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
	free(cntrl_response_header);
	
	memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload,8);
	free(cntrl_response_payload);
	printf("%s",cntrl_response);
	fflush(stdout);
	
	
	sendALL(sock_index, cntrl_response, response_len);
	for(i=0;i<5;i++){
	//sendUDP(sock[i],cntrl_response,response_len,i);
	}
	
	free(cntrl_response);
}

void router_update(char *payload){
int add =0x08;
int i,j;
for(i=0;i<5;i++){
	uint16_t response_len,payload_len;
	char *cntrl_response_header,*cntrl_response_payload, *cntrl_response;
	memcpy(&router[i].router_id, payload+add, sizeof(router[i].router_id));
	memcpy(&router[i].nexthop_id, payload+add+0x04, sizeof(router[i].router_id));
	memcpy(&router[i].cost, payload+add+0x06, sizeof(router[i].router_id));
	add = add+0x06;	
	for(j=0;j<=5;j++){
		if(tab[i][j].id == router[i].router_id){
		int arrcost = router[i].cost;
		if (arrcost < tab[i][j].cost)
		tab[i][j+1].cost = arrcost;
		tab[i][j+1].next_hop = router[i].router_id;	
		}
	}
	//Create Payload
	cntrl_response_payload = (char *) malloc(8);
	//Payload
	for(i=0;i<5;i++){
		for(j=0;j<=5;j++){
		int add =0x00;
		memcpy(cntrl_response_payload+add,&tab[i][j+1].id, sizeof(tab[i][j].id));
		memcpy(cntrl_response_payload+add,&tab[i][j+1].padding, sizeof(tab[i][j].id));
		memcpy(cntrl_response_payload+add,&tab[i][j+1].next_hop, sizeof(tab[i][j].id));
		memcpy(cntrl_response_payload+add,&tab[i][j+1].cost, sizeof(tab[i][j].id));
		add = add+0x08;		
		}
	}
	for(j=0;j<5;j++){
	for(i=0;i<=5;i++){
	if(tab[j][i+1].cost == 0){
	//Router_ID
	int k;
	for(k=0;k<5;k++){
//	tab[j][i].id=initres[0].router_id;
	initres[i].router_id=tab[k][i].id;
	//Next Hop
//	if(payload[i].cost!=65535)
	initres[i].nexthop_id=tab[k][i].next_hop;
	//Cost
	initres[i].cost=tab[k][i].cost;
	printf("\n%d %d %d ",initres[i].router_id,initres[i].nexthop_id,initres[i].cost);
	fflush(stdout);	
	}}}}
	response_len=sizeof(cntrl_response_payload);
	for(i=0;i<5;i++){
	sendUDP(sock[i],cntrl_response_payload,response_len,i);
	}
	

}}

void cost_update(int sock_index,char *cntrl_payload){
	uint16_t payload_len, response_len;
	char *cntrl_response_header, *cntrl_response_payload, *cntrl_response;	
	int id;
	int cost;
	int j,i;
	memcpy(&id, cntrl_payload, sizeof(id));
	memcpy(&cost, cntrl_payload+0x02, sizeof(cost));
	for(i=0;i<5;i++){
		for(j=0;j<=5;j++){
		if(tab[i][0].id == router_id){
			if(j == id)
			tab[i][j].cost=cost;
		}
		}
	}
	//Create Payload
	cntrl_response_payload = (char *) malloc(8);
	//Payload
	for(i=0;i<5;i++){
		for(j=0;j<=5;j++){
		int add =0x00;
		memcpy(cntrl_response_payload+add,&tab[i][j+1].id, sizeof(tab[i][j].id));
		memcpy(cntrl_response_payload+add,&tab[i][j+1].padding, sizeof(tab[i][j].id));
		memcpy(cntrl_response_payload+add,&tab[i][j+1].next_hop, sizeof(tab[i][j].id));
		memcpy(cntrl_response_payload+add,&tab[i][j+1].cost, sizeof(tab[i][j].id));
		add = add+0x08;		
		}
	}
	for(j=0;j<5;j++){
	for(i=0;i<=5;i++){
	if(tab[j][i+1].cost == 0){
	//Router_ID
	int k;
	for(k=0;k<5;k++){
//	tab[j][i].id=initres[0].router_id;
	initres[i].router_id=tab[k][i].id;
	//Next Hop
//	if(payload[i].cost!=65535)
	initres[i].nexthop_id=tab[k][i].next_hop;
	//Cost
	initres[i].cost=tab[k][i].cost;
	printf("\n%d %d %d ",initres[i].router_id,initres[i].nexthop_id,initres[i].cost);
	fflush(stdout);	
	}}}}
	response_len=sizeof(cntrl_response_payload);
	for(i=0;i<5;i++){
	sendUDP(sock[i],cntrl_response_payload,response_len,i);
	}	
}	

