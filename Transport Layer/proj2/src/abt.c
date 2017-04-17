#include "../include/simulator.h"
#include<stdio.h>
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
/*struct msg {
char data[20];
};

struct pkt {
int seqnum;
int acknum;
int checksum;
char payload[20];
}*/

struct pkt last_pckt,rec_pckt;
#define RTT 20 
/********* STUDENTS WRITE THE NEXT SIX ROUTINES *********/
int ack=0,seq=0,seqb=0;
int ack_rec=0;

/* called from layer 5, passed the data to be sent to other side */
void A_output(message)
  struct msg message;
{
struct pkt pckt;
if(ack_rec == 0){
                printf("\n Sending Packet\n");
                //Creating Packet
                pckt.seqnum=seq;
                char a[25];
                strcpy(a,message.data);
                a[19]='\0';
                strcpy(pckt.payload,a);
                pckt.checksum=checksum(pckt);
                //printf("\n Payload  %s",(pckt.payload));
                //Duplicate for retx
                last_pckt=pckt;
                //Sending to layer 3
                tolayer3 (0,pckt);
                starttimer (0,RTT);
//              seq=inverse(seq);
                ack_rec=1;
        }
else{
printf("\n Waiting for ack");
}
}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(packet)
  struct pkt packet;
{
if (check(packet) && packet.acknum == seq && ack_rec==1 ){
                stoptimer(0);
                printf("\n ACK recieved \n");
                //Creating message
                seq=inverse(seq);
                ack_rec=0;
        }
else{
      printf("\n ACK error...\n");    
}
}

/* called when A's timer goes off */
void A_timerinterrupt()
{
      printf("\nTimeout Resending the packets\n");
        tolayer3 (0,last_pckt);
        starttimer (0,RTT);
	ack_rec=1;

}

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
seq=0;
ack=0;
//checksum=0;
}

/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(packet)
  struct pkt packet;
{
if (check(packet) && packet.seqnum == seqb){
      printf("\nPacket recieved successfully\n ");
        struct msg message;
        strcpy(message.data,packet.payload);
 //       rec_pckt =packet;
        tolayer5 (1, message.data);
        seqb=inverse(seqb);
        struct pkt ackpckt;
        ackpckt.acknum=packet.seqnum;
        ackpckt.checksum=(checksum(ackpckt));
        tolayer3(1,ackpckt);
	rec_pckt=ackpckt;
}
else if(check(packet)) {
tolayer3(1,rec_pckt);
}
}

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
seqb =0;
}

//Create checksum
int checksum(struct pkt pckt){
                int ackseq=0;
                int checksum=0;
                checksum=pckt.seqnum+pckt.acknum;
                int i;
                for (i = 0; i < 20; i++)
                checksum =checksum + (int)pckt.payload[i];
                return checksum;
                }
int check(struct pkt pckt)
{
int checksum;
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

int inverse(int i)
{
if (i==0)
return 1;
else
return 0;
}

