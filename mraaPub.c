/*
* This code uses mraa hardware library to publish data from an analog pin to a remote broker.
* Lincenses :
            Paho -> http://www.eclipse.org/legal/
            Mraa -> Intel's Terms of Use
        
Compile -> cc -o mraaPub mraaPub.c -lmraa -lpaho-mqtt3a

* Authored by :
              Arthur Kennedy Otieno Jonyo
              (c) 2017-07-18
              (c) Africa's Talking 2017

*/
//Global Libs
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//Mraa Specific Libs and Header Files

// #include <stdio.h>
#include <unistd.h>
#include "mraa/aio.h"
#include <signal.h>

//Paho Specific Libs and Header Files
#include "MQTTAsync.h"

#if !defined(WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif

//MQTT Specific Vars
#define ADDRESS     "tcp://sungura1-angani-ke-host.africastalking.com"
#define CLIENTID    "Edison"
#define TOPIC       "Host"
#define QOS         1
#define TIMEOUT     10000L
char PAYLOAD [41];

//Mraa Variables
mraa_aio_context adc_a0;
int adc_value = 0;
adc_a0 = mraa_aio_init(0);
adc_value = mraa_aio_read(adc_a0);


//MQTT Specific Funcs
volatile MQTTAsync_token deliveredtoken;

int finished = 0;

void connlost(void *context, char *cause)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
	int rc;

	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);

	printf("Reconnecting\n");
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start connect, return code %d\n", rc);
 		finished = 1;
	}
}


void onDisconnect(void* context, MQTTAsync_successData* response)
{
	printf("Successful disconnection\n");
	finished = 1;
}


void onSend(void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
	int rc;

	printf("Message with token value %d delivery confirmed\n", response->token);

	opts.onSuccess = onDisconnect;
	opts.context = client;

	if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start sendMessage, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}


void onConnectFailure(void* context, MQTTAsync_failureData* response)
{
	printf("Connect failed, rc %d\n", response ? response->code : 0);
	finished = 1;
}

void onConnect(void* context, MQTTAsync_successData* response)
{
	MQTTAsync client = (MQTTAsync)context;
	MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
	MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
	int rc;

	printf("Successful connection\n");
	sprintf(PAYLOAD,"%d",adc_value);
	opts.onSuccess = onSend;
	opts.context = client;

	pubmsg.payload = PAYLOAD;
	pubmsg.payloadlen = strlen(PAYLOAD);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	deliveredtoken = 0;

	if ((rc = MQTTAsync_sendMessage(client, TOPIC, &pubmsg, &opts)) != MQTTASYNC_SUCCESS)
	{
		printf("Failed to start sendMessage, return code %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

int running = 0;

void sig_handler(int signo)
{
    if(signo == SIGINT)
    {
        printf("Exiting...\n");
        running =-1;
    }
}

int main()
{
    
    // mraa_aio_context adc_a0;
    // uint16_t adc_value = 0;
    // float adc_value_float =0.0;
    mraa_result_t r = MRAA_SUCCESS;

    // adc_a0 = mraa_aio_init(0);
    if(adc_a0 == NULL)
    {
            return 1;

    }

signal(SIGINT,sig_handler);

    while(running == 0)
    {
        //adc_value = mraa_aio_read(adc_a0);
        //adc_value_float = mraa_aio_read_float(adc_a0);
                fprintf(stdout, "ADC A0 read %d\n",adc_value);
        // fprintf(stdout, "ADC A0 read float - %.5f (Ctrl+C to exit)\n",adc_value_float);
        //usleep(1500000);
                sprintf(PAYLOAD,"%d",adc_value);
                MQTTAsync client;
                MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
                MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
                MQTTAsync_token token;
                int rc;

                MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

                MQTTAsync_setCallbacks(client, NULL, connlost, NULL, NULL);

                conn_opts.keepAliveInterval = 20;
                conn_opts.cleansession = 1;
                conn_opts.onSuccess = onConnect;
                conn_opts.onFailure = onConnectFailure;
                conn_opts.context = client;
                if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
                {
                    printf("Failed to start connect, return code %d\n", rc);
                    exit(EXIT_FAILURE);
                }

                printf("Waiting for publication of %s\n"
                    "on topic %s for client with ClientID: %s\n",
                    PAYLOAD, TOPIC, CLIENTID);
                while (!finished)
                    #if defined(WIN32)
                        Sleep(100);
                    #else
                        usleep(10000L);
                    #endif
    }
    MQTTAsync_destroy(&client);
    r =  mraa_aio_close(adc_a0);
    if(r != MRAA_SUCCESS)
    {
        mraa_result_print(r);
    }
    return r;

}