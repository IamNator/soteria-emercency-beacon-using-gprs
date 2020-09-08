

#ifndef PUBLISH_FUNCTION_H_INCLUDED
#define PUBLISH_FUNCTION_H_INCLUDED
#include "conMqttPass.h"


void serialComs(int , int , int); //(int baud_Serial_monitor, int baud_sim_module, int delay_micro)

bool isGPRSReady(); //checks if gprs connection is established

void sendMQTTMessage(char* , char* , char* , char* , char* , char* , char* ,int); //(char* clientId, char* brokerUrl, char* brokerPort, char* topic, char* message, char* username, char* password) send message using MQTT protocol


#endif // PUBLISH_FUNCTION_H_INCLUDED
