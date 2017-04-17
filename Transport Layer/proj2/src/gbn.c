#include "../include/simulator.h"
#include <stdio.h>
#include<string.h>
/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional data transfer 
   protocols (from A to B). Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
    (although some can be lost).
**********************************************************************/

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
//Sender
#define RTT 20
int nextseqnum = 1;
int base =1;

int N;
//end=N;
//Reciever
int expected=1;
//Sender wndow
struct pkt pckt[2000],ackpckt;
struct msg buffer[1000];
int start=0,buffno=0;
int b=0;
void A_output(message)
  struct msg message;
{
//pckt[nextseqnum].payload[19]='\0';
//strcpy(pckt[nextseqnum].payload,message.data); 

if (nextseqnum < (base+N)){
	pckt[nextseqnum].payload[19]='\0';
	strcpy(pckt[nextseqnum].payload,message.data);
	pckt[nextseqnum].seqnum=nextseqnum;
        pckt[nextseqnum].checksum=checksum(pckt[nextseqnum]);
//	printf("\n at a%d",pckt[nextseqnum].seqnum);
                
        //Sending to layer 3
        tolayer3 (0,pckt[nextseqnum]);
        if (base == nextseqnum)
        starttimer (0,RTT);
        nextseqnum++;
	}
else
{
strcpy(buffer[start].data,message.data);
start++;
buffno++;
}
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
  struct pkt packet;
{
struct pkt pckt;
printf("\n%d",packet.checksum);
printf("\n%d",checksum(packet));
if (packet.checksum == checksum(packet))
{		base = packet.acknum+1;
                if (base == nextseqnum)
                stoptimer(0);
                else {
                stoptimer(0);
                starttimer(0,RTT);
		if (buffno > 0 && nextseqnum <(base+N))
			{
			A_output(buffer[b]);
			b++;
			buffno--;
			}			
                }
}
}
/* called when A's timer goes off */
void A_timerinterrupt()
{
int r;
r=0;
starttimer(0,RTT);
for (r=base;r<nextseqnum;r++){
tolayer3 (0,pckt[r]);
//starttimer(0,25.0);
}
//printf("\n time interrupt-a\n");
}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
N=getwinsize();
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)
  struct pkt packet;
{
//struct pkt ackpckt;
if (check(packet) && packet.seqnum == expected){
 //     printf("\nPacket recieved successfully\n ");
        struct msg message;
        strcpy(message.data,packet.payload);
 //       rec_pckt =packet;
        tolayer5 (1, message.data);
        expected++;
        //struct pkt ackpckt;
        ackpckt.acknum=packet.seqnum;
	ackpckt.seqnum=expected;
        ackpckt.checksum=checksum(ackpckt);
	tolayer3(1,ackpckt);
}
else{
tolayer3(1,ackpckt);
}
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
expected =1;
ackpckt.acknum=0;
ackpckt.seqnum=expected;
ackpckt.checksum=checksum(ackpckt);
tolayer3(1,ackpckt);
}

int checksum(struct pkt pckt){
                int checksum=0;
                checksum=pckt.seqnum+pckt.acknum;
                int i;
                for (i = 0; i < 20; i++)
                checksum =checksum + (int)pckt.payload[i];
                return checksum;
                }
int check(struct pkt pckt)
{
int checksum=0;
int ackseq=0;
                checksum=pckt.seqnum+pckt.acknum;
                int i;
                for (i = 0; i < 20; i++)
                checksum =checksum + (int)pckt.payload[i];
if(checksum == pckt.checksum)
return 1;
else
return 0;
}

