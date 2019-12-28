

#ifndef PUBLISH_FUNCTION_H_INCLUDED
#define PUBLISH_FUNCTION_H_INCLUDED

#include "mqtt.h"

void serialComs(int baud_Serial_monitor, int baud_sim_module, int delay_micro);

void ShowSerialData(); //reads from sim module and print on the screen

bool isGPRSReady(); //checks if gprs connection is established

void sendMQTTMessage(char* clientId, char* brokerUrl, char* brokerPort, char* topic, char* message); //send mqtt message to mqtt


#endif // PUBLISH_FUNCTION_H_INCLUDED
