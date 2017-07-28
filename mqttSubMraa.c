/*
This code uses Paho C client Library to subscribe to a topic "AfricasTalking",
then switches on or off an LED based on "ON" or "OFF" commands from this topic.

T&Cs:
Africa's Talking ->https://www.africastalking.com/terms
Intel            ->https://www.intel.com/content/www/us/en/legal/terms-of-use.html
Eclipse IOT      ->http://www.eclipse.org/legal/termsofuse.php
                 ->http://www.eclipse.org/legal/

Author:
        Arthur Kennedy Otieno Jonyo
        git:TheBeachmaster
        www.otienoken.me
        (c) 2017-07-26
        (c) Africa's Talking

*/

/*
Paho Client Libs
*/
#include "stdio.h"
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

