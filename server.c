/*
 * message server application sample
 *
 * This application will receive commands from a client,
 * and provide responses (integer and string). It is 
 * a demonstration of simple message passing support in Unix
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*------Global Variables--------*/
typedef struct queue
{
	float charge[10];
	int duration[10];
	int front,rear;
}q;
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

/**************Generate number**********/

double generate()
{
	 double n,j=999999;
	
	j=j*10000;
	
				//Declare variable to hold seconds on clock.
	time_t seconds;
				//Get value from system clock and place in seconds variable.
	time(&seconds);
	n=seconds%9999;
	j=j+n;

	return j;
}

/*-------------New user---------------*/

double newuser(MSGENVELOPE asd)
{
	FILE *fp1;
	FILE *fp2;
			
		asd.data.number=generate();;		

		asd.data.validuser=1;
		if(asd.data.type==1)
			asd.data.balance=100;
		else
			asd.data.balance=0;

		if(asd.data.type==1)
		{
			fp1=fopen("Prepaid.txt","a");
			fwrite(&asd,1,sizeof(asd),fp1);		
			fclose(fp1);
		}
		else if(asd.data.type==2)
		{
			fp2=fopen("Postpaid.txt","a");
			fwrite(&asd,1,sizeof(asd),fp2);		
			fclose(fp2);
		}
		
		return(asd.data.number);
}

/*Search existing user based on phone number*/
MSGENVELOPE search(MSGENVELOPE asd)
{
	MSGENVELOPE temp;		

	FILE *fp1;
	FILE *fp2;
	
	if(asd.data.type==1)
	{
		fp1=fopen("Prepaid.txt","r");		
		while(!feof(fp1))
		{		
	
			fread(&temp,1,sizeof(temp),fp1);
		
			if(temp.data.number==asd.data.number)
			{
				temp.data.validuser=1;
				fclose(fp1);				
				return(temp);
			}
		}
	
		fclose(fp1);
	}
	else if(asd.data.type==2)
	{

		fp2=fopen("Postpaid.txt","r");		
		while(!feof(fp2))
		{		
	
			fread(&temp,1,sizeof(temp),fp2);
		
			if(temp.data.number==asd.data.number)
			{
				temp.data.validuser=1;			
				fclose(fp2);				
				return(temp);
			}
		}
		
		fclose(fp2);
	}
	return(asd);
}	


MSGENVELOPE update(MSGENVELOPE asd)
{
	FILE *fp1;
	FILE *fp2;	
	MSGENVELOPE temp,s;

	temp=search(asd);
	temp.data.balance=asd.data.balance;
	temp.data.durcha.charge[(temp.data.durcha.front)]=asd.data.durcha.charge[(temp.data.durcha.front)];
	temp.data.durcha.duration[(temp.data.durcha.front)]=asd.data.durcha.duration[(temp.data.durcha.front)];
	temp.data.durcha.front=asd.data.durcha.front;
	temp.data.durcha.rear=asd.data.durcha.rear;
	fp1=fopen("temp","w");
	
	if(asd.data.type==1)
	    	fp2=fopen("Prepaid.txt","r");
	else
		fp2=fopen("Postpaid.txt","r");

	while(1)
    	 {
       		fread(&s,1,sizeof(s),fp2);
       		if(feof(fp2))
	 		break;
       		if(s.data.number!=asd.data.number)
	 		fwrite(&s,1,sizeof(s),fp1);
     	}
    	fclose(fp2);
    	fclose(fp1);

	if(asd.data.type==1)
	    	fp2=fopen("Prepaid.txt","w");
	else
		fp2=fopen("Postpaid.txt","w");

    	fp1=fopen("temp","r");
	while(1)
     	{
		fread(&s,1,sizeof(s),fp1);
		if(feof(fp1))
		  break;
		fwrite(&s,1,sizeof(s),fp2);
     	}

        fwrite(&temp,1,sizeof(temp),fp2);
	fclose(fp2);
    	fclose(fp1);

	return(temp);
}

/*======== To send the balance =========*/

MSGENVELOPE showbalance(MSGENVELOPE asd)
{ 		

	MSGENVELOPE temp;
	
	temp=search(asd);
	return(temp);
	
	
}


int main (void)
{
	key_t message_key;
	int mid = -1; // message ID
	int rc,z;	// return code from message processing
	int done = 0;
	MSGENVELOPE msg,temp,temp1;
	FILE *postpaid;
	FILE *prepaid;


//==============Initialisation=============//


	message_key = ftok (".", 'A');
	if (message_key == -1) { 
		printf ("cannot allocate key\n");
		return 1;
	}	/* endif */


	/*
	 * if message queue exists, use it, or
	 * allocate a new one
	 */

	if ((mid = msgget (message_key, 0)) == -1) {
		printf ("no queue available, create!\n");

		/*
		 * nope, let's create one (user/group read/write perms)
		 */

		mid = msgget (message_key, IPC_CREAT | 0660);
		if (mid == -1) {
			printf ("cannot allocate a new queue!\n");
			return 2;
		}	/* endif */
	}	/* endif */

	printf ("our message queue ID is %d\n", mid);

	/*
	 * we now have a message queue ID, use it to
	 * send/receive messages
	 */

	
	while (!done) {
		rc = msgrcv (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0, 0);
		if (rc == -1)
			break;

		printf ("received: type: %d command: %d\n", msg.type, msg.data.message_command);

		//************ structure containts *************//
		FILE *read;
		read=fopen("temp.txt","w");
		
		fprintf(read,"\nname=%s",msg.data.name);
		fprintf(read,"\naddress=%s",msg.data.addrs);
		fprintf(read,"\nnumber=%10.0f",msg.data.number);
		fprintf(read,"\ntype=%d",msg.data.type);
		fprintf(read,"\nbalance=%f",msg.data.balance);
		fprintf(read,"\ntimer=%f",msg.data.timer);
		fprintf(read,"\nrecharge=%d",msg.data.recharge);
		fprintf(read,"\nvaliduser=%d",msg.data.validuser);
		fprintf(read,"\ncity=%s",msg.data.city);
		fprintf(read,"\nstate=%s",msg.data.state);
		fprintf(read,"\nnationality=%s",msg.data.nationality);
		fprintf(read,"\npincode=%ld",msg.data.pincode);
		fprintf(read,"\ngender=%s",msg.data.gender);
		fprintf(read,"\ndob=%s",msg.data.dob);
		fclose(read);
		//***********************************************//

		switch (msg.data.message_command) {

		case 1:	
			msg.data.number=newuser(msg);
			
			rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
			if (rc == -1)
				printf("\nError in sending.");		
			
			break;

		case 2:

			msg=search(msg);

			rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
			if (rc == -1)
				printf("\nError in sending.");	
			
			break;

		case 3:			
			msg=update(msg);
			
			rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
			if (rc == -1)
				printf("\nError in sending.");	
			
			break;

		case 4:
			
			temp=showbalance(msg);						
			
			rc = msgsnd (mid, (void *)&msg, sizeof (SAMPLEMESSAGE), 0);
			if (rc == -1)
				printf("\nError in sending.");	
			break;
		
		default:done=1;
			break;
		}	/* endif */

		// send result
}


	msgctl (mid, IPC_RMID, (struct msqid_ds *)NULL);
	return 0;
}	/* end main */



