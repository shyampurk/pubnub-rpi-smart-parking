#include "pubnub_sync.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int uart0_filestream = -1;

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

int pubnub_start(char *a)
{
        enum pubnub_res res;
        char const *chan = "RadioStudio";
        pubnub_t *pbp = pubnub_alloc();
        struct Pubnub_UUID uuid;
        struct Pubnub_UUID_String str_uuid;
        if (NULL == pbp)
        {
                printf("Failed to allocate Pubnub context!\n");
                return -1;
        }
        pubnub_init(pbp, "pub-c-a1f796fb-1508-4c7e-9a28-9645035eee90", "sub-c-d4dd77a4-1e13-11e5-9dcf-0619f8945a4f");
        if (0 != pubnub_generate_uuid_v4_random(&uuid))
        {
                pubnub_set_uuid(pbp, "zeka-peka-iz-jendeka");
        }
        else
        {
                str_uuid = pubnub_uuid_to_string(&uuid);
                pubnub_set_uuid(pbp, str_uuid.uuid);
                printf("Generated UUID: %s\n", str_uuid.uuid);
        }

        pubnub_set_auth(pbp, "danaske");
        puts("Publishing...");
        res = pubnub_publish(pbp, chan, a);
        if(res != PNR_STARTED)
        {
                printf("pubnub_publish() returned unexpected: %d\n", res);
                pubnub_free(pbp);
                return -1;
        }
        res = pubnub_await(pbp);
        if (res == PNR_STARTED)
        {
                printf("pubnub_await() returned unexpected: PNR_STARTED(%d)\n", res);
                pubnub_free(pbp);
                return -1;
        }
        if (PNR_OK == res) {
        printf("Published! Response from Pubnub: %s\n", pubnub_last_publish_result(pbp));
        }
        else if (PNR_PUBLISH_FAILED == res) {
        printf("Published failed on Pubnub, description: %s\n", pubnub_last_publish_result(pbp));
        }
        else {
        printf("Publishing failed with code: %d\n", res);
        }
        return 0;
}

int main()
{
        uartInit();
        int status = 0;
        int laststatus = 0;
        int laststatus1 = 0;
        int laststatus2 = 0;
        //char const *msg;
        while(1)
        {
                usleep(10000);
                if (uart0_filestream != -1)
                {
                        unsigned char rx_buffer[1];
                        int rx_length = read(uart0_filestream,(void*)rx_buffer, 1);

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
                        status = atoi(rx_buffer);
                        printf("%d\n",status);
                }
                if(status < 4 && status != laststatus)
                {
                        if(status == 1){
                        pubnub_start("{\"Requester\":\"DEVICE\",\"Device ID\":\"001\",\"Request Type\":1,\"Request Value\":1}");
                        laststatus = status;
                        }
                        else if(status == 2){
                        pubnub_start("{\"Requester\":\"DEVICE\",\"Device ID\":\"001\",\"Request Type\":2,\"Request Value\":1}");
                        laststatus = status;
                        }
                        else if (status == 3){
                        pubnub_start("{\"Requester\":\"DEVICE\",\"Device ID\":\"001\",\"Request Type\":1,\"Request Value\":0}");
                        laststatus = status;
                        }
                }
                else if(status > 3 && status < 7 && status != laststatus1)
                {
                        if(status == 4){
                        pubnub_start("{\"Requester\":\"DEVICE\",\"Device ID\":\"002\",\"Request Type\":1,\"Request Value\":1}");
                        laststatus1 = status;
                        }
                        else if(status == 5){
                        pubnub_start("{\"Requester\":\"DEVICE\",\"Device ID\":\"002\",\"Request Type\":2,\"Request Value\":1}");
                        laststatus1 = status;
                        }
                        else if(status == 6){
                        pubnub_start("{\"Requester\":\"DEVICE\",\"Device ID\":\"002\",\"Request Type\":1,\"Request Value\":0}");
                        laststatus1 = status;
                        }
                }
                else if(status > 6 && status < 10 && status != laststatus2)
                {
                        if(status == 7){
                        pubnub_start("{\"Requester\":\"DEVICE\",\"Device ID\":\"003\",\"Request Type\":1,\"Request Value\":1}");
                        laststatus2 = status;
                        }
                        else if(status == 8){
                        pubnub_start("{\"Requester\":\"DEVICE\",\"Device ID\":\"003\",\"Request Type\":2,\"Request Value\":1}");
                        laststatus2 = status;
                        }
                        else if(status == 9){
                        pubnub_start("{\"Requester\":\"DEVICE\",\"Device ID\":\"003\",\"Request Type\":1,\"Request Value\":0}");
                        laststatus2 = status;
                        }
                }
        usleep(500000);
        }
        close(uart0_filestream);
        return 0;
}

