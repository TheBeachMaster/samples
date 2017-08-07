#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://sungura1-angani-ke-host.africastalking.com:1883"
#define CLIENTID    "EdisonSync"
#define TOPIC       "Host"
//#define PAYLOAD     "Hello Edison"
#define QOS         1
#define TIMEOUT     10000L



int main(int argc,char * arg[])
{
    MQTTClient client;
    MQTTClient_message  message = MQTTClient_message_initializer;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int topicLen;
    int ch;
    MQTTClient_create(&client, ADDRESS, CLIENTID,MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    do
    {
        MQTTClient_receive(client,&TOPIC,&topicLen,&message,2000);
        printf("Received Data from %s: \n",TOPIC);
        printf("Contents are %s \n",(char*)(message->payload));
        MQTTClient_freeMesage(&message);
        MQTTClient_free(&topic);
    }while((rc = MQTTClient_connect(client,&conn_opts)) && (rc = MQTTClient_subscribe(client,TOPIC,QOS))) == MQTTCLIENT_SUCCESS)
    MQTTClient_disconnect(client,10000);
    MQTTClient_destroy(&client);
    return rc;
}