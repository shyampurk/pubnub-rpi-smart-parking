/*********************************************************************************
SMART PARKING LOT SYSTEM
*********************************************************************************/
//Import the Libraries Required 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <termios.h> 
#include <pthread.h>
#include "pubnub_sync.h"

//Individual Parking ID's for the Parking LOT's 
const char *g_lot1 = "001";
const char *g_lot2 = "002";
const char *g_lot3 = "003";

/*Characted String used to form the json data to be sent 
to the app using json_data function */
char g_jsonResponse[26] = "";

int g_uart0_filestream = -1;
int g_firstChar = 0,g_secondChar = 0,g_thirdChar = 0;

//Function Prototypes used in this program
void *pubnub_receive(void* p_unused);
void prepare_json_data(int p_status1,int p_status2,int p_status3);
int pubnub_send(char *p_data);

/**************************************************************************************

Function Name 		:	uartInit
Description		:	Initialize the UART Serial Communication between the 
				Raspberry Pi and the Atmega 8a Microcontroller
Parameters 		:	void
Return 			:	int - when uart connection fails returns -1 else 0

**************************************************************************************/

int uartInit(void)
{
	g_uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
	if(g_uart0_filestream == -1)
	{
		printf("Error Connecting with the Device \n");
		return -1;
	}
	//Setting the Baud Rate and No. of the Stop Bits for the UART
	struct termios options;
	tcgetattr(g_uart0_filestream,&options);
	//BAUD Rate Initialized to 9600 bps
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(g_uart0_filestream, TCIFLUSH);
	tcsetattr(g_uart0_filestream,TCSANOW,&options);
	return 0;
}

/*****************************************************************************************

Function Name 		:	pubnub_receive
Description		:	Suscribe from the "parkingstatus-req" channel using the 
				pubnub and The Request is sent fromt the Android App to 
				get the present status of the parking lots.
				This is seperate thread along with the main thread.
Parameters 		:	void
Return 			:	(void *) when the pubnub connection fails returns -1

****************************************************************************************/

void *pubnub_receive(void* p_unused)
{
	char const *l_msg = NULL;
	enum pubnub_res l_res;
	char const *l_requestChannel = "parkingstatus-req";
	pubnub_t *l_receive = pubnub_alloc();
	if (NULL == l_receive) 
	{
		printf("Failed to allocate Pubnub context!\n");
		return (void *)-1;
	}
	pubnub_init(l_receive, "demo", "demo");
	while(1)
    	{
		//puts("Subscribing...");
		l_res = pubnub_subscribe(l_receive, l_requestChannel, NULL);
		
		if (l_res != PNR_STARTED) 
		{
	    		printf("pubnub_subscribe() returned unexpected: %d\n", l_res);
	    		pubnub_free(l_receive);
	    		return (void *)-1;
		}

		l_res = pubnub_await(l_receive);
		if (l_res == PNR_STARTED) 
		{
	    		printf("pubnub_await() returned unexpected: PNR_STARTED(%d)\n", l_res);
	    		pubnub_free(l_receive);
	    		return (void *)-1;
		}
		if (PNR_OK == l_res) 
		{
	    		puts("Subscribed!");
		}
		else 
		{
	    		printf("Subscribing failed with code: %d\n", l_res);
		}
		l_res = pubnub_subscribe(l_receive, l_requestChannel, NULL);
		if (l_res != PNR_STARTED) 
		{
	        	printf("pubnub_subscribe() returned unexpected: %d\n", l_res);
	        	pubnub_free(l_receive);
	        	return (void *)-1;
	    	}
		
		l_res = pubnub_await(l_receive);
		if (l_res == PNR_STARTED) 
		{
	        	printf("pubnub_await() returned unexpected: PNR_STARTED(%d)\n", l_res);
	        	pubnub_free(l_receive);
	        	return (void *)-1;
	  	}
		if (PNR_OK == l_res) 
		{
	        	puts("Subscribed! Got messages:");
		        for (;;) 
		        {
		        	l_msg = pubnub_get(l_receive);
		            	if (NULL == l_msg) 
		            	{
		                	break;
		            	}
		            	puts(l_msg);
			    	if(l_msg[48] == '4')
			    	{
					    pubnub_send(g_jsonResponse);
					    memset(g_jsonResponse, 0, sizeof(g_jsonResponse));
			    	}
		  	}
	    	}
	    	else 
	    	{
	    		printf("Subscribing failed with code: %d\n", l_res);
	    	} 
	}
	pubnub_free(l_receive);
    	return NULL;
}

/******************************************************************************************

Function Name 		:	pubnub_send
Description		:	Publish the present status of the parking lots to the 
				Requested App
Parameters 		:	p_data
		p_data  :	Parameter is the char pointer holds the data has to be 
				sent to the Requested App
Return 			:	int, if error in sent thr function returns -1 else 0

*****************************************************************************************/

int pubnub_send(char *p_data)
{
	enum pubnub_res l_response;
	char const *l_responseChannel = "parkingstatus-resp";
	struct Pubnub_UUID uuid;
	struct Pubnub_UUID_String str_uuid;
	pubnub_t *l_publish = pubnub_alloc();
	if (NULL == l_publish) 
	{
		printf("Failed to allocate Pubnub context!\n");
		return -1;
	}
	pubnub_init(l_publish, "demo", "demo");

	if (0 != pubnub_generate_uuid_v4_random(&uuid)) 
	{
		pubnub_set_uuid(l_publish, "zeka-peka-iz-jendeka");
	}
	else 
	{
		str_uuid = pubnub_uuid_to_string(&uuid);
		pubnub_set_uuid(l_publish, str_uuid.uuid);
		printf("Generated UUID: %s\n", str_uuid.uuid);
	}

	pubnub_set_auth(l_publish, "danaske");
	puts("Publishing...");
	l_response = pubnub_publish(l_publish, l_responseChannel, p_data);
	if(l_response != PNR_STARTED) 
	{
		printf("pubnub_publish() returned unexpected: %d\n", l_response);
		pubnub_free(l_publish);
		return -1;
	}
	l_response = pubnub_await(l_publish);
	if (l_response == PNR_STARTED) 
	{
		printf("pubnub_await() returned unexpected: PNR_STARTED(%d)\n", l_response);
		pubnub_free(l_publish);
		return -1;
	}
	if (PNR_OK == l_response) {
	printf("Published! Response from Pubnub: %s\n", pubnub_last_publish_result(l_publish));
	return 0;
	}
	else if (PNR_PUBLISH_FAILED == l_response) {
	printf("Published failed on Pubnub, description: %s\n", pubnub_last_publish_result(l_publish));
	}
	else {
	printf("Publishing failed with code: %d\n", l_response);
	}
	pubnub_free(l_publish);
	return 0;
}

/***************************************************************************************

Function Name 		:	prepare_json_data
Description		:	With the Present Status of the Parking Lots 
				this function makes a json data to be sent as Response
Parameters 		:	p_status1,p_status2,p_status3
	p_status1	:	Status of the first Parking Lot
	p_status2	:	Status of the second Parking Lot
	p_status3	:	Status of the third Parking Lot
Return 			:	void

***************************************************************************************/

void prepare_json_data(int p_status1,int p_status2,int p_status3)
{
	memset(g_jsonResponse, 0, sizeof(g_jsonResponse));
	char l_buf [2] = "";
	strcat(g_jsonResponse,"{\"");
	strcat(g_jsonResponse,g_lot1);
	strcat(g_jsonResponse,"\":");
	snprintf(l_buf,sizeof(l_buf),"%d",p_status1);
	strcat(g_jsonResponse,l_buf);
	strcat(g_jsonResponse,",\"");
	strcat(g_jsonResponse,g_lot2);
	strcat(g_jsonResponse,"\":");
	snprintf(l_buf,sizeof(l_buf),"%d",p_status2);	
	strcat(g_jsonResponse,l_buf);
	strcat(g_jsonResponse,",\"");
	strcat(g_jsonResponse,g_lot3);
	strcat(g_jsonResponse,"\":");
	snprintf(l_buf,sizeof(l_buf),"%d",p_status3);	
	strcat(g_jsonResponse,l_buf);
	strcat(g_jsonResponse,"}");
}

/****************************************************************************************

Function Name 		:	main
Description		:	Initalize UART, Thread and publish if any status change
				in the parking lots
Parameters 		:	void
Return 			:	int, if error in the function returns -1 else 0

****************************************************************************************/

int main(void)
{
	pthread_t thread_id;
	if(uartInit() == 0)
	{
		pthread_create(&thread_id,NULL,&pubnub_receive,NULL);
		long l_laststatus = 0;
		long l_status = 0;
		while(1)
	    	{
			if (g_uart0_filestream != -1)
			{
				char *l_ptr = NULL;
				char l_rxBuffer[4];
				int l_rxLength = read(g_uart0_filestream,(void*)l_rxBuffer, 4);
				if (l_rxLength > 0)
				{
					l_rxBuffer[l_rxLength] = '\0';
				}
				/*DATA RECEIVED by UART is 1,2,3
					1	-	Parking LOT is Free
					2	-	Parking LOT is Filled
					3	-	Fault in the Sensor	*/
				g_firstChar  = l_rxBuffer[0] - '0';
				g_secondChar = l_rxBuffer[1] - '0';
				g_thirdChar  = l_rxBuffer[2] - '0';
				l_status = strtol(l_rxBuffer,&l_ptr,10);
				printf("%ld\n",l_status);
				/*DATA SENT to the APP is Modified to 0,1,2
					0	-	Parking LOT is Free
					1	-	Parking LOT is Filled
					2	-	Fault in the Sensor */
				prepare_json_data(g_firstChar-1,g_secondChar-1,g_thirdChar-1);
			}
			if(l_status != l_laststatus)
			{
				l_laststatus = l_status;
				pubnub_send(g_jsonResponse);
				memset(g_jsonResponse, 0, sizeof(g_jsonResponse));
			}
	        	usleep(5000000);
		}
	    	close(g_uart0_filestream);
	}
	else
	{
		printf("UART Initialization Failed, Aborting");
		return -1;
	}
	pthread_exit(NULL);
	return 0;
}

//End of the Program
