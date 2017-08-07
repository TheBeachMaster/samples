#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://sungura1-angani-ke-host.africastalking.com:1883"
#define CLIENTID    "EdisonSync"
//#define TOPIC       "Host"
//#define PAYLOAD     "Hello Edison"
#define QOS         1
#define TIMEOUT     10000L

char payLoadData[BUFSIZ];

int rc;
int topicLen;
char* TOPIC = "Host";
int ch;


int main(void)
{
    MQTTClient client;
    MQTTClient_message*  message;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&client, ADDRESS, CLIENTID,MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_connect(client,&conn_opts);
    MQTTClient_subscribe(client,TOPIC,QOS);
    do
    {
        MQTTClient_receive(client,&TOPIC,&topicLen,&message,2000);
        printf("Received Data from %s: \n",TOPIC);
        //(char*)(message->payload)
        sprintf(payLoadData,"%s",(char*)message->payload);
        printf("Contents are %s \n",payLoadData);
        MQTTClient_freeMessage(&message);
        MQTTClient_free(&TOPIC);
    } while((rc = MQTTClient_connect(client,&conn_opts)) && (rc = MQTTClient_subscribe(client,TOPIC,QOS)) == MQTTCLIENT_SUCCESS);
    MQTTClient_disconnect(client,10000);
    MQTTClient_destroy(&client);
    return rc;
}