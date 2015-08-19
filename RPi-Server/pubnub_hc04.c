//Import the Libraries Required 
#include "pubnub_sync.h" 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <termios.h> 
#include <pthread.h>

int uart0_filestream = -1;
int AppData;

//UART Function to receive the data from the AVR Chip
void uartInit(void)
{
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);
	if(uart0_filestream == -1)
	{
		printf("Error Connecting with the Device \n");
	}

	struct termios options;
	tcgetattr(uart0_filestream,&options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream,TCSANOW,&options);
}

void* pubnub_receive(void* unused)
{
	char const *msg;
        enum pubnub_res res;
        char const *chan = "parkingstatus-req";
        pubnub_t *pbp = pubnub_alloc();
        if (NULL == pbp) 
        {
                printf("Failed to allocate Pubnub context!\n");
                return NULL;
        }
        pubnub_init(pbp, "demo", "demo");

	puts("Subscribing...");
	res = pubnub_subscribe(pbp, chan, NULL);
    	if (res != PNR_STARTED) {
        printf("pubnub_subscribe() returned unexpected: %d\n", res);
        pubnub_free(pbp);
        return NULL;
    	}
    	res = pubnub_await(pbp);
    	if (res == PNR_STARTED) {
        printf("pubnub_await() returned unexpected: PNR_STARTED(%d)\n", res);
        pubnub_free(pbp);
        return NULL;
	}
	if (PNR_OK == res) {
        puts("Subscribed!");
    	}
    	else {
        printf("Subscribing failed with code: %d\n", res);
    	}
	res = pubnub_subscribe(pbp, chan, NULL);
	if (res != PNR_STARTED) {
        printf("pubnub_subscribe() returned unexpected: %d\n", res);
        pubnub_free(pbp);
        return NULL;
    	}
	res = pubnub_await(pbp);
	if (res == PNR_STARTED) {
        printf("pubnub_await() returned unexpected: PNR_STARTED(%d)\n", res);
        pubnub_free(pbp);
        return NULL;
  	}
	if (PNR_OK == res) {
        puts("Subscribed! Got messages:");
        for (;;) {
            msg = pubnub_get(pbp);
            if (NULL == msg) {
                break;
            }
            puts(msg);
	    printf("%c",msg[48]);
	    if(msg[48] == '4')
	    {
		AppData = 1;
	        printf("%d",AppData);
	    }
	  }
    	}
    	else {
        		printf("Subscribing failed with code: %d\n", res);
    	} 
	pthread_exit(NULL);
}

//Function used to Publish the data to the PUBNUB 
int pubnub_start(char *a)
{
	enum pubnub_res res;
	char const *chan = "parkingstatus-resp";
	pubnub_t *pbp2 = pubnub_alloc();
	struct Pubnub_UUID uuid;
	struct Pubnub_UUID_String str_uuid;
	if (NULL == pbp2) 
	{
		printf("Failed to allocate Pubnub context!\n");
		return -1;
	}
	pubnub_init(pbp2, "demo", "demo");

	if (0 != pubnub_generate_uuid_v4_random(&uuid)) 
	{
		pubnub_set_uuid(pbp2, "zeka-peka-iz-jendeka");
	}
	else 
	{
		str_uuid = pubnub_uuid_to_string(&uuid);
		pubnub_set_uuid(pbp2, str_uuid.uuid);
		printf("Generated UUID: %s\n", str_uuid.uuid);
	}

	pubnub_set_auth(pbp2, "danaske");
	puts("Publishing...");
	res = pubnub_publish(pbp2, chan, a);
	if(res != PNR_STARTED) 
	{
		printf("pubnub_publish() returned unexpected: %d\n", res);
		pubnub_free(pbp2);
		return -1;
	}
	res = pubnub_await(pbp2);
	if (res == PNR_STARTED) 
	{
		printf("pubnub_await() returned unexpected: PNR_STARTED(%d)\n", res);
		pubnub_free(pbp2);
		return -1;
	}
	if (PNR_OK == res) {
	printf("Published! Response from Pubnub: %s\n", pubnub_last_publish_result(pbp2));
	}
	else if (PNR_PUBLISH_FAILED == res) {
	printf("Published failed on Pubnub, description: %s\n", pubnub_last_publish_result(pbp2));
	}
	else {
	printf("Publishing failed with code: %d\n", res);
	}
	return 0;
}

//Main Function 
int main()
{
        pthread_t thread_id;
	uartInit();
	int status = 0;
	int laststatus = 0;
	pthread_create(&thread_id,NULL,&pubnub_receive,NULL);
        while(1)
        {
                up: usleep(10000);
                //Receive the data from the UART Rx
                if (uart0_filestream != -1)
		{
			unsigned char rx_buffer[2];
			int rx_length = read(uart0_filestream,(void*)rx_buffer, 2);
			int x,y;
			if (rx_length < 0)
			{
			}
			else if (rx_length == 0)
			{
				//No data waiting
			}
			else
			{
				rx_buffer[rx_length] = '\0';
			}
			y = rx_buffer[0] - 48;
			x = rx_buffer[1] - 48;
			status = (y*10)+x;
			printf("%d\n",status);
		}
		if(AppData == 1 || status != laststatus)
		{
			AppData = 0;
			laststatus = status;
			if(status == 1){
			pubnub_start("{\"001\":1,\"002\":1,\"003\":1}");
			}
			else if(status == 2){
			pubnub_start("{\"001\":1,\"002\":1,\"003\":0}");
			}
			else if (status == 3){
			pubnub_start("{\"001\":1,\"002\":0,\"003\":1}");
			}
			else if(status == 4){
			pubnub_start("{\"001\":1,\"002\":0,\"003\":0}");
			}
			else if(status == 5){
			pubnub_start("{\"001\":0,\"002\":1,\"003\":1}");
			}
			else if(status == 6){
			pubnub_start("{\"001\":0,\"002\":1,\"003\":0}");
			}
			else if(status == 7){
			pubnub_start("{\"001\":0,\"002\":0,\"003\":1}");
			}
			else if(status == 8){
			pubnub_start("{\"001\":0,\"002\":0,\"003\":0}");
			}
			else if(status == 9){
			pubnub_start("{\"001\":2,\"002\":1,\"003\":1}");
			}
			else if (status == 10){
			pubnub_start("{\"001\":2,\"002\":1,\"003\":0}");
			}
			else if(status == 11){
			pubnub_start("{\"001\":2,\"002\":0,\"003\":1}");
			}
			else if(status == 12){
			pubnub_start("{\"001\":2,\"002\":0,\"003\":0}");
			}
			else if(status == 13){
			pubnub_start("{\"001\":1,\"002\":2,\"003\":1}");
			}
			else if(status == 14){
			pubnub_start("{\"001\":1,\"002\":2,\"003\":0}");
			}
			else if(status == 15){
			pubnub_start("{\"001\":0,\"002\":2,\"003\":1}");
			}
			else if(status == 16){
			pubnub_start("{\"001\":0,\"002\":2,\"003\":0}");
			}
			else if(status == 17){
			pubnub_start("{\"001\":1,\"002\":1,\"003\":2}");
			}
			else if(status == 18){
			pubnub_start("{\"001\":1,\"002\":0,\"003\":2}");
			}
			else if(status == 19){
			pubnub_start("{\"001\":0,\"002\":1,\"003\":2}");
			}
			else if(status == 20){
			pubnub_start("{\"001\":0,\"002\":0,\"003\":2}");
			}
			else if(status == 21){
			pubnub_start("{\"001\":2,\"002\":2,\"003\":1}");
			}
			else if(status == 22){
			pubnub_start("{\"001\":2,\"002\":2,\"003\":0}");
			}
			else if(status == 23){
			pubnub_start("{\"001\":1,\"002\":2,\"003\":2}");
			}
			else if(status == 24){
			pubnub_start("{\"001\":0,\"002\":2,\"003\":2}");
			}
			else if(status == 25){
			pubnub_start("{\"001\":2,\"002\":1,\"003\":2}");
			}
			else if(status == 26){
			pubnub_start("{\"001\":2,\"002\":0,\"003\":2}");
			}
			else if(status == 27){
			pubnub_start("{\"001\":2,\"002\":2,\"003\":2}");
			}
			else goto up;
			pthread_create(&thread_id,NULL,&pubnub_receive,NULL);			
		}
	usleep(500000);
      	}
        close(uart0_filestream);
	pthread_exit(NULL);
	return 0;
}

//End of the Program
