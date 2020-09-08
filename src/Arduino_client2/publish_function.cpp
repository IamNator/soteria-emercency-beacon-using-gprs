#include <Arduino.h>
#include <SoftwareSerial.h>
#include "mqtt.h"
#include "publish_function.h"

SoftwareSerial GPRS(4,5); //RX TX


byte mqttMessage[127];
//byte === uint8_t

void serialComs(int baud_Serial_monitor, int baud_sim_module, int delay_micro){ //initial serial commnication
   
     Serial.begin(baud_Serial_monitor);
     GPRS.begin(baud_sim_module);
     delay(delay_micro);

}


void ShowSerialData() //note .read() reads one byte/char at a time, 
{
  while(GPRS.available()!=0)  /* If data is available on serial port */
  Serial.write(char (GPRS.read())); /* Print character received on to the serial monitor */
}

bool isGPRSReady(){

   GPRS.println("AT");
   GPRS.println("AT");
   GPRS.println("AT+CGATT?");
   int index = 0;
   char data1;
   char * gprsStr;

   while (GPRS.available()){
      data1 = (char)GPRS.read();
      Serial.write(data1);
      gprsStr[index++] = data1;
   }

      Serial.println("Check OK");
      Serial.print("gprs str = ");
      Serial.println(gprsStr);

   if (gprsStr=="OK"){
      Serial.println("GPRS OK");
   return true;
   }
   else {
      Serial.println("GPRS NOT OK");
   return false;
   }

   Serial.println("send AT to wake up GPRS");

}

void sendMQTTMessage(char* clientId, char* brokerUrl, char* brokerPort, char* topic, char* message){
    
     char atCommand[50];
     GPRS.println("AT"); // Sends AT command to wake up cell phone
     delay(1000); // Wait a second
    // digitalWrite(13, HIGH);

     GPRS.println("AT+CSTT=\"gloflat\",\"flat\",\"flat\""); // Puts phone into GPRS mode
     Serial.println("AT+CSTT=\"gloflat\",\"flat\",\"flat\""); //AT+ CSTT=<apn>, <user id>, <password>
     delay(2000); // Wait a second

     GPRS.println("AT+CIICR"); //Bring up wireless connection with GPRS
     Serial.println("AT+CIICR");
     delay(2000);

     GPRS.println("AT+CIFSR"); //get local IP Address
     Serial.print("AT+CIFSR :");
   //   Serial.println(  ( while(GPRS.available) return GPRS.read(); )  );
     delay(2000);
     ShowSerialData();

/***********************Send TCP/IP messages****************************/   

     strcpy(atCommand, "AT+CIPSTART=\"TCP\",\"");
     strcat(atCommand, brokerUrl);
     strcat(atCommand, "\",\"");
     strcat(atCommand, brokerPort);
     strcat(atCommand, "\"");
     //atCommand = "AT+CIPSTART=\"TCP\",\"" + brokerUrl + "\",\"" + brokerPort + "\"" ;
     GPRS.println(atCommand);
     Serial.println(atCommand);
     //Serial.println("AT+CIPSTART=\"TCP\",\""+brokerUrl+"\",\""+brokerUrl+"\"");
     delay(2000);

     GPRS.println("AT+CIPSEND");
     Serial.println("AT+CIPSEND");
     delay(2000);

     int mqttMessageLength = 16 + strlen(clientId);
     Serial.println(mqttMessageLength);
     mqtt_connect_message(mqttMessage, clientId);

     for (int j = 0; j < mqttMessageLength; j++) {
         GPRS.write(mqttMessage[j]); // Message contents
         Serial.write(mqttMessage[j]); // Message contents
         Serial.println("");
     }

     GPRS.write(byte(26)); // (signals end of message)
     Serial.println("Sent");
     delay(10000);

     GPRS.println("AT+CIPSEND");
     Serial.println("AT+CIPSEND");
     delay(2000);
     mqttMessageLength = 4 + strlen(topic) + strlen(message);
     Serial.println(mqttMessageLength);
     mqtt_publish_message(mqttMessage, topic, message);
     for (int k = 0; k < mqttMessageLength; k++) {
        GPRS.write(mqttMessage[k]);
        Serial.write((byte)mqttMessage[k]);
     }

     GPRS.write(byte(26)); // (signals end of message)

/***********************************/

     Serial.println("-------------Sent-------------"); // Message contents
     delay(5000);
     GPRS.println("AT+CIPCLOSE");
     Serial.println("AT+CIPCLOSE");
     delay(2000);
 
 }
