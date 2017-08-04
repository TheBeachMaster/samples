#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://sungura1-angani-ke-host.africastalking.com:1883"
#define CLIENTID    "EdisonSync"
#define TOPIC       "Host"
#define PAYLOAD     "Hello Edison"
#define QOS         1
#define TIMEOUT     10000L



int main(int argc,char * arg[])
{
    MQTTClient client;
    MQTTClient_message  message = MQTTClient_message_initializer;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    // MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    MQTTClient_subscribe(client, TOPIC, QOS);
    MQTTClient_receive(client,TOPIC,strlen(PAYLOAD),&message,TIMEOUT);

    printf("Received... %s\n",(char*)message->payload);

    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    
    do 
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return rc;
}