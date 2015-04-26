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

	//printf ("my PID is %d\n", getpid());
	//printf ("message queue ID is %d\n", mid);


		//=======Input details of new user=========//

		msg.data.message_command = 1;
		fflush(stdin);
		system("clear");
		system("zenity\ --info\ --text=\"\nWELCOME NEW USER\n\"\ --title=\"MOBILE BILLING USING MESSAGE PASSING\"\ --height=150\ --width=300");

		msg.data.message_command = 1;
		fflush(stdin);
		system("zenity\ --info\ --text=\"\nPLEASE ENTER THE FOLLOWING DETAILS\n\"\ --title=\"ENTER THE DETAILS\"\ --height=150\ --width=100");
		printf("Enter the name:");

		gets(msg.data.name);
		printf("\nEnter the address:");
		gets(msg.data.addrs);
		printf("\nEnter the Date of Birth:");
		gets(msg.data.dob);
		printf("\nEnter the gender:");
		fflush(stdin);		
		gets(msg.data.gender);
		printf("\nEnter the city:");
		gets(msg.data.city);
		printf("\nEnter the state:");
		gets(msg.data.state);
		printf("\nEnter the Nationality:");
		gets(msg.data.nationality);
		printf("\nEnter the Pincode:");
		scanf("%ld",&msg.data.pincode);
		printf("\nSelect the connection type\n");
		printf("\t1.Prepaid\n\t2.Postpaid\n\nChoice:");
		scanf("%d",&type);
	
		if(type==1)
			msg.data.type=1;
		else if	(type==2)
			msg.data.type=2;
		else
					system("zenity\ --warning\ --text=\"Wrong Choice!!\"");		


		/*send the user details*/
		rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
		if (rc == -1) {
			system("zenity --warning --text=\"SEND ERROR!!\"");

			return 4;
		}	
		
		/*receive the num */
		rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
		if (rc == -1) {
			system("zenity\ --warning\ --text=\"RECEIVE ERROR!!\"");
			return 5;
		}
		system("zenity\ --info\ --text=\"\n\t\tCONGRATULATIONS!!\t\t\nYOUR NEW MOBILE NO. IS GENERATED AS SHOWN INTERMINAL!!\n\"\ --title=\"SUCCESSFUL CREATION\"\ --height=150\ --width=100");

		printf("\n\n\t\t\tYour number is :%10.0f\n\n\n\n",msg.data.number);


	return 0;
}	/* end main */

