

#ifndef PUBLISH_FUNCTION_H_INCLUDED
#define PUBLISH_FUNCTION_H_INCLUDED

#include "conMqttPass.h"

//int indexof(String text, S/tring pattern);

void UPDATE_SERIAL();

void serialComs(int baud_Serial_monitor, int baud_sim_module, int delay_micro);

String ShowSerialData(); //reads from sim module and print on the screen

bool isGPRSReady(); //checks if gprs connection is established

void sendMQTTMessage(char* clientId, char* brokerUrl, char* brokerPort, char* topic, char* message, char* username, char* password); //send mqtt message to mqtt


#endif // PUBLISH_FUNCTION_H_INCLUDED
