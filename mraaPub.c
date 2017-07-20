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
#include "MQTTClient.h"

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



int finished = 0;



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

//MQTT
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    MQTTClient_create(&client, ADDRESS, CLIENTID,
    MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    

//Mraa Variables
mraa_aio_context adc_a0;
int adc_value = 0;
float adc_value_float =0.0;
mraa_result_t r = MRAA_SUCCESS;

adc_a0 = mraa_aio_init(0);
if(adc_a0 == NULL){
return 1;
}

signal(SIGINT,sig_handler);

    while(running == 0)
    {
                if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
                {
                    printf("Failed to connect, return code %d\n", rc);
                    exit(EXIT_FAILURE);
                }
                fprintf(stdout, "ADC A0 read %d\n",adc_value);
                sprintf(PAYLOAD,"%d",adc_value);
                //  sprintf(PAYLOAD,"%d",dataVal);
                pubmsg.payload = PAYLOAD;
                pubmsg.payloadlen = strlen(PAYLOAD);
                pubmsg.qos = QOS;
                pubmsg.retained = 0;
                MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
                printf("Waiting for up to %d seconds for publication of %s\n"
                        "on topic %s for client with ClientID: %s\n",
                        (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
                rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
                printf("Message with delivery token %d delivered\n", token);
                
    }
    r =  mraa_aio_close(adc_a0);
    if(r != MRAA_SUCCESS)
    {
        mraa_result_print(r);
    }
    //return r;
     MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;

}