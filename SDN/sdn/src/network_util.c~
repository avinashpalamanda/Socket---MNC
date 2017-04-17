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

#include <stdlib.h>
#include<stdio.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "../include/control_header_lib.h"


ssize_t recvALL(int sock_index, char *buffer, ssize_t nbytes)
{
    ssize_t bytes = 0;
    bytes = recv(sock_index, buffer, nbytes, 0);
    
    if(bytes == 0) return -1;
    while(bytes != nbytes)
        bytes += recv(sock_index, buffer+bytes, nbytes-bytes, 0);


    return bytes;
        fflush(stdout);
	printf("hi");

}
ssize_t recvudp(int sock_index, char *buffer)
{
    ssize_t bytes = 0;
    struct sockaddr_storage their_addr;
    int addr_len = sizeof their_addr;
    if ((bytes = recvfrom(sock_index, buffer, 1000 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
    perror("recvfrom");
    exit(1);
    }
    return bytes;
}

ssize_t sendUDP(int sock_index,char *buffer, ssize_t nbytes,int PORT,int i){
    ssize_t bytes = 0;
    	
    int len=sizeof(update_addr[0]);
    update_addr[i].sin_addr.s_addr = inet_addr("127.0.0.1");
    if((sendto(sock_index, buffer, sizeof(buffer),0,(struct sockaddr *) &update_addr[0],len))==-1)
    {
	printf("fail");
	}	

    /*if(bytes == 0) return -1;
    while(bytes != nbytes)
        bytes += sendto(sock_index, buffer+bytes, nbytes-bytes, 0);
	*/
    return bytes;

}


ssize_t sendALL(int sock_index, char *buffer, ssize_t nbytes)
{
    ssize_t bytes = 0;
    bytes = send(sock_index, buffer, nbytes, 0);

    if(bytes == 0) return -1;
    while(bytes != nbytes)
        bytes += send(sock_index, buffer+bytes, nbytes-bytes, 0);

    return bytes;
}
