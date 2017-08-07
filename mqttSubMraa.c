/*
This code uses Paho C client Library to subscribe 
to a topic "AfricasTalking",
then switches on or off an LED based on "ON" or "OFF" 
commands from this topic.

T&Cs:
Africa's Talking ->https://www.africastalking.com/terms
Intel            ->https://www.intel.com/content/www/us/en/legal/terms-of-use.html
Eclipse IOT      ->http://www.eclipse.org/legal/termsofuse.php
                 ->http://www.eclipse.org/legal/

Author:
        Arthur Kennedy Otieno Jonyo
        git:TheBeachmaster
        https://www.otienoken.me
        (c) 2017-07-26
        (c) Africa's Talking

        ->Updated on August 3rd 2017

*/

/*
Paho Client Libs
*/
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"

/*
Mraa and Mraa GPIO libs
*/

#include <mraa.h>
#include <mraa/gpio.h> 


/*
Params for the broker-client connnection
*/

#define ADDRESS     "tcp://sungura1-angani-ke-host.africastalking.com:1883"
#define CLIENTID    "Edison"
#define TOPIC       "AfricasTalking"
#define PAYLOAD     "Edsion Payload!"
#define QOS         1
#define TIMEOUT     10000L

//Set up Payload Variables and commands to expect
char payLoadData [BUFSIZ];
char *offCmd = "OFF";
char *onCmd = "ON";

/*
We'll use these variables to check if the 
comparisons check out;
should return 0 if so,else something else will be done
*/
int onRes;
int offRes;

mraa_gpio_context m_gpio;
m_gpio = mraa_gpio_init(5);
mraa_gpio_dir(m_gpio, MRAA_GPIO_OUT);

//We'll be dealing with a QoS1 payload...So we'll set that up
volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

/*
When an OFF message arrives we'll blink our LED twice
then switch it OFF.
When an ON message arrives we'll blink once then ON
If we can not understand the payload, we'll maintain "state"
*/
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
        //TABS over SPACES !!!
        int state;
        char* payloadptr;
        
        //We'll first get our message and spit it out
        printf("New mesage\n");
        printf("         from the topic: %s\n",topicName);
        sprintf(payLoadData,"%s",(char*)message->payload);
        printf("         message contents: %s\n",payLoadData);

        //Let's inspect our payload
        onRes = strcmp(onCmd,payLoadData);//Compare Payload Data with ON
        offRes = strcmp(offCmd,payLoadData);//Compare Payload Data with OFF

        //Check our Payloads
        if(onRes == 0) //We have recived ON as  command
        {
                /*We could essentially write to the pin here but
                let's experiment with this state thing...
                */
                state = 1;
        } else if(offRes == 0) //We received an OFF Command
        {
                //Ditto...
                state = 0;
        } else 
        {
                //Ditto...
                state = -1;
                printf("Unknown Command\n");
        }

        do 
        {
                //Turn LED ON
                mraa_gpio_write(m_gpio, 1);
                sleep(1);
                mraa_gpio_write(m_gpio, 0);
                sleep(1);
                mraa_gpio_write(m_gpio, 1);
                
        } while(state == 1);
       
           do 
        {
                //Turn LED OFF
                mraa_gpio_write(m_gpio, 0);
                sleep(1);
                mraa_gpio_write(m_gpio, 1);
                sleep(1);
                mraa_gpio_write(m_gpio, 0);
                sleep(1);
                mraa_gpio_write(m_gpio, 1);
                sleep(1);
                mraa_gpio_write(m_gpio, 0);
                
        } while(state == 0);
        mraa_gpio_close(m_gpio);
        
        /*
        Clean up and Yield
        */
        MQTTClient_freeMessage(&message);
        MQTTClient_free(topicName);
        return 1;
}

/*
We lost connection
*/
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(void)
{
        
    
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);

    do 
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
