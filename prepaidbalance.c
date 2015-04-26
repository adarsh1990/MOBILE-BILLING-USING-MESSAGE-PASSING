/*
 * message client application sample
 *
 * This application will talk to a message server
 * and request some simple operations of it. This is
 * a demonstration of simple message passing support in Unix
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
typedef struct queue
{
	float charge[10];
	int duration[10];
	int front,rear;
}q;

/*
 * note: our client and server must agree upon this structure!
 */
int t;
int c;
typedef struct tagSAMPLEMESSAGE {
	int message_command;
	char name[25];
	char gender[5];
	char dob[12];
	char addrs[25];
	char city[15];
	char state[15];
	char nationality[15];
	long int pincode;
	int type;//1-prepaid 2-postpaid
	char prooftype[20];	
	float balance;
	double timer;
	double number;
	int validuser;
	int recharge;
	q durcha;
} SAMPLEMESSAGE;


typedef struct tagMSGENVELOPE {
	long type;
	SAMPLEMESSAGE data;
} MSGENVELOPE;

//========Timer==================//
double generate_time()
{
	 double n;
	 time_t seconds;
	 time(&seconds);
	 n=seconds;
       	 return n;
}

int generate_no()
{
	 double j;
	 int n;
	
time_t seconds;

time(&seconds);

j=seconds%9999;
n=(int)j;

return n;
}

int main (void)
{

	key_t message_key;
	int mid; // message ID
	int rc;	// return code from message processing
	int done = 0;
	int count = 0;
	MSGENVELOPE msg;
	char buffer[100];
	int x,temp,select,type,user;
	double dest;
	float recharge=0.0;
	double t1,t2;
	int d,cardnum;
	float charge;
	int min,sec,l;

	// ======== Initial value===============//
	msg.data.validuser=0;
	msg.data.recharge=0;
	strcpy(msg.data.name,"!!!!!!!");
	strcpy(msg.data.addrs,"@@@@@@");
	type=0;//1-prepaid 2-postpaid
	strcpy(msg.data.prooftype,"######");	
	msg.data.balance=0;
	msg.data.timer=0;
	msg.data.number=0;
	msg.data.durcha.front=0;
	msg.data.durcha.rear=0;
	msg.data.durcha.charge[10]=0,0,0,0,0,0,0,0,0,0;
	msg.data.durcha.duration[10]=0,0,0,0,0,0,0,0,0,0;
	strcpy(msg.data.gender,"!");
	msg.data.dob[12]="00/00/0000";
	strcpy(msg.data.city,"!!!!!!!!!!!");
	strcpy(msg.data.state,"!!!!!!!!!!!");
	strcpy(msg.data.nationality,"!!!!!!!!!!!");
	msg.data.pincode=000000;
	//========= Initial value==============//


	/*
	 * open the same message key/queue as server
	 */

	message_key = ftok (".", 'A');
	if (message_key == -1) { 
		printf ("cannot allocate key\n");
		return 1;
	}	/* endif */


	/*
	 * if message queue exists, use it, or
	 * allocate a new one
	 */

	mid = msgget (message_key, 0);

			type=msg.data.type=1;

			msg.data.message_command = 2;
		system("clear");
		system("zenity\ --info\ --text=\"\nWELCOME TO PREPAID BALANCE ENQUIRY \n\"\ --title=\"MOBILE BILLING USING MESSAGE PASSING\"\ --height=150\ --width=100");

			printf("Enter your number:");
			scanf("%10lf",&msg.data.number);
			
			/*send message to check the number */

			rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
			if (rc == -1) {
				system("zenity --warning --text=\"SEND ERROR!!\"");
			return 4;
			}	
			
			/*receive info on validity*/
			rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
			if (rc == -1) {
			system("zenity\ --warning\ --text=\"RECEIVE ERROR!!\"");
			return 5;
			}		
				
		

			if(msg.data.validuser==1)
			{
				system("zenity\ --info\ --text=\"\n\t\tCONGRATULATIONS!!\t\nYOU ARE VALID USER!!\n\"\ --title=\"SUCCESSFUL ENTRY\"\ --height=150\ --width=100");


									msg.data.message_command=4;
									/*send msg to read the balance*/
									rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
									if (rc == -1) {
										system("zenity --warning --text=\"SEND ERROR!!\"");
										return 4;
										}										
									/*Recieve message about the balance*/
									rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
									if (rc == -1) {
											system("zenity\ --warning\ --text=\"RECEIVE ERROR!!\"");;
											return 5;
									}
printf("\nYour current Balance is : Rs. %f ",msg.data.balance);
		
								system("zenity\ --info\ --text=\"\nYOUR CURRENT BALANCE IS AS SHOWN IN THE TERMINAL\"\ --height=150\ --width=100");	
								


			}
else
{
system("zenity --info --title=\"INVALID NUMBER---PLEASE TRY AGAIN\"");
}/*end if (vaild user)*/
												

		


	return 0;
}	/* end main */

