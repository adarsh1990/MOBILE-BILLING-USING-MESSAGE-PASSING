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

	printf ("1 ... New User\n");
	printf ("2 ... Old User\n");
	
	printf ("choice: ");
	scanf("%d",&x);

	msg.type = 1;
	
		
	switch (x) {
	case 1:
		//=======Input details of new user=========//

		msg.data.message_command = 1;
		fflush(stdin);
		printf("\nEnter the name:");
		scanf("%s",msg.data.name);
		printf("\nEnter the address:");
		scanf("%s",msg.data.addrs);
		printf("\nEnter the Date of Birth:");
		scanf("%s",msg.data.dob);
		printf("\nEnter the gender");
		fflush(stdin);		
		scanf("%s",msg.data.gender);
		printf("\nEnter the city:");
		scanf("%s",msg.data.city);
		printf("\nEnter the state:");
		scanf("%s",msg.data.state);
		printf("\nEnter the Nationality:");
		scanf("%s",msg.data.nationality);
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
			printf("\nWrong Choice");	


		/*send the user details*/
		rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
		if (rc == -1) {
			printf ("send error!\n");
			return 4;
		}	
		
		/*receive the num */
		rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
		if (rc == -1) {
			printf ("receive error!\n");
			return 5;
		}

		printf("\n\nYour number is :%10.0f",msg.data.number);
		break;

	case 2:				
		printf("\nSelect the connection type\n");
		printf("\t1.Prepaid\n\t2.Postpaid\n\nChoice:");
		scanf("%d",&type);
		
		if(type==1)
			msg.data.type=1;
		else if	(type==2)
			msg.data.type=2;
		else
			printf("\nWrong Choice");

		
			msg.data.message_command = 2;

			printf("Enter ur number");
			scanf("%10lf",&msg.data.number);
			
			/*send message to check the number */

			rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
			if (rc == -1) {
			printf ("send error!\n");
			return 4;
			}	
			
			/*receive info on validity*/
			rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
			if (rc == -1) {
				printf ("receive error!\n");
			return 5;
			}		
				
		

			if(msg.data.validuser==1)
			{
				printf("\nUr a vaild user");

				while(1)
				{
					if(type==1)
					{
						printf("\nSelect ur choice:");
						printf("\n1.Call");
						printf("\n2.Check Balance");
						printf("\n3.Recharge");					
					}
					else if(type ==2)						
					{
						printf("\nSelect ur choice:");
						printf("\n1.Call");
						printf("\n2.Generate Bill");
						printf("\n3.Pay Bill");
					}
					printf("\n4. Exit(0)");
						
					scanf("%d",&select);
						
						if(select==1)
						{
							if(type==1 && msg.data.balance<=0)
							{
								printf("Insufficient balance");
								break;
							}	

							printf("\nEnter the Destination no:\n");
							scanf("%lf",&dest);
							
							printf("\nPress 1 to make the call\n");
							scanf("%d",&temp);
							fflush(stdin);

							if(temp == 1)
							{
								t1=generate_time();
								
								printf("End call(Press any number)");
								scanf("%d",&d);
								t2=generate_time();
								
								t2=t2-t1;
								
								if(t2/60<1)
								{
									printf("\nThe call duration is%10.0lf seconds\n",t2);		

									printf("Your call charge is: Rs 0.50\n");
									charge=(0.50);
	
								}	
								else
								{
									min=(int)t2/60;
									sec=(int)t2%60;

									printf("\nThe call duration is%d minutes %d seconds \n",min,sec);
		
									charge= (0.50)*(min + 1);
									printf("Your call charge is: Rs %f\n",charge);
								}
					
								msg.data.timer=t2;
								
								if(msg.data.type==1)					
									msg.data.balance-=charge;
								else if(msg.data.type==2)
									msg.data.balance+=charge;
								msg.data.durcha.charge[msg.data.durcha.front]=charge;
								msg.data.durcha.duration[msg.data.durcha.front]=(int)t2;
								(msg.data.durcha.front)++;
								msg.data.durcha.front%=10;
								if((msg.data.durcha.front)==(msg.data.durcha.rear))
									{
										(msg.data.durcha.rear)++;
										msg.data.durcha.rear%=10;
									}

								msg.data.message_command=3;								
								/*send mgs to update the file */

								rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
								if (rc == -1) {
										printf ("send error!\n");
										return 4;
										}	
								
								/*receive the updated the file */
								rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
									if (rc == -1) {
											printf ("receive error!\n");
											return 5;
									}		
								
								if(type==1)
								{	
									msg.data.message_command=4;
									
									/*send msg to read the balance*/			
									rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
									if (rc == -1) {
										printf ("send error!\n");
										return 4;
										}
								
									/*recieve msg to read the balance*/
									rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
									if (rc == -1) {
											printf ("receive error!\n");
											return 5;
									}									

									printf("\n\nUr current balance is :%f",msg.data.balance);
									for(l=msg.data.durcha.rear;l!=msg.data.durcha.front;)
									{
										printf("\n%f\t%d",msg.data.durcha.charge[l],msg.data.durcha.duration[l]);
										l=(l+1)%10;
									}
								}																									
								
							}/*end if(temp)*/

						}
						else if(select==2)
						{
							if(type ==1)
								{
									msg.data.message_command=4;
									/*send msg to read the balance*/
									rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
									if (rc == -1) {
										printf ("send error!\n");
										return 4;
										}										
									/*Recieve message about the balance*/
									rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
									if (rc == -1) {
											printf ("receive error!\n");
											return 5;
									}									
								printf("Ur current Balance is %f ",msg.data.balance);
								}		
							else if(type==2)
								{
									msg.data.message_command=4;
									/*send the msg to generate the bill*/
									rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
									if (rc == -1) {
										printf ("send error!\n");
										return 4;
										}	

									/*receive the generated the bill details*/
									rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
									if (rc == -1) {
											printf ("receive error!\n");
											return 5;
											}
									printf("\nBill number:",generate_no());
									printf("\n\nName");	
									printf("%s",msg.data.name);
									printf("\nPhone Number:");
									printf("%10.0f",msg.data.number);
									printf("\nAddress:%s",msg.data.addrs);
									printf("\nTotal charge:%f",msg.data.balance);
									printf("\n\nTotal duration:%f",msg.data.timer);
									printf("\nLast 10 calls:\n");
									printf("\nCharge\t\tTime\n");
									for(l=msg.data.durcha.rear;l!=msg.data.durcha.front;)
									{
										printf("\n%f\t\t%d\n",msg.data.durcha.charge[l],msg.data.durcha.duration[l]);
										l=(l+1)%10;
									}
								}
							
						}						
						else if (select==3)
						{	
							if(type==1)
							{
								msg.data.message_command=3;							
								printf("\nEnter the recharge amount:");
								scanf("%f",&recharge);
								msg.data.balance+=recharge;
								msg.data.recharge=1;

								/*send the updated balance amount */
								rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
									if (rc == -1) {
										printf ("send error!\n");
										return 4;
										}

								/*receive the updated balance amount */
								rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
									if (rc == -1) {
											printf ("receive error!\n");
											return 5;
											}
								printf("\nYour account has been recharged successfully.");
								printf("\nYour account balance is %f",msg.data.balance);			
							}
							else if(type==2)
							{
								msg.data.message_command=3;							
								printf("\nEnter ur credit card number:");
								scanf("%d",&cardnum);
								
								msg.data.balance=0.0;
								
								/*send the msg to pay the bill*/
								rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
									if (rc == -1) {
										printf ("send error!\n");
										return 4;
										}

								/*receive the msg*/
								rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
									if (rc == -1) {
											printf ("receive error!\n");
											return 5;
											}
								printf("\nYour bill is paid successfully");
								
							}
						}
						else
							exit(0);									
					}/*end while*/

				}/*end if (vaild user)*/
												
		default :exit(0);
		
	}/*end switch*/	

	return 0;
}	/* end main */

