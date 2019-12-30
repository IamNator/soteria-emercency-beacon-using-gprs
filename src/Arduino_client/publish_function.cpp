#include <Arduino.h>
#include <SoftwareSerial.h>
//#include <iostream>/
#include "conMqttPass.h"
#include "publish_function.h"

SoftwareSerial GPRS(3,2); //to TX_A6, to RX_A6


byte mqttMessage[127];
//byte === uint8_t


///int indexof(string text, string pattern){
    //where appears the pattern in the text?
//    int loc = text.std::find(pattern, 0);/
//    if (loc != string::npos)/
//    {/
//      re/turn loc;
//    }/
//    else/{
//      retur/n -1;
//    }/
//}/


void UPDATE_SERIAL()
{
  delay(500);
  while (Serial.available()) 
  {
    GPRS.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(GPRS.available()) 
  {
    Serial.write(GPRS.read());//Forward what Software Serial received to Serial Port
  }
}


void serialComs(int baud_Serial_monitor, int baud_sim_module, int delay_micro){ //initial serial commnication
   
     Serial.begin(baud_Serial_monitor);
     GPRS.begin(baud_sim_module);
     delay(delay_micro);

}

String ShowSerialData() {
  String reply = "";
  if (GPRS.available())  {
    reply = GPRS.readString();
  }
    Serial.print("Reply: ");
    Serial.println(reply);
  return reply;
}


void sendMQTTMessage(char* clientId, char* brokerUrl, char* brokerPort, char* topic, char* message, char* username, char* password){
    
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
   //   Serial.print("AT+CIFSR :");
   //   Serial.println(  ( while(GPRS.available) return GPRS.read(); )  );
     delay(2000);


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

     int mqttMessageLength = 20 + strlen(clientId) + strlen(username) + strlen(password);
     
     Serial.println(mqttMessageLength);
     delay(50);
     
     mqtt_connect_message(mqttMessage, clientId, username, password);

     char mesage[60];
     int j;

      for (j = 0; j < mqttMessageLength; j++) { //Prints to Sim module
         mesage[j] = mqttMessage[j]; // Message contents
      }

      mesage[++j] = '\0';
      String meg = mesage;
      Serial.println(meg);

//   /  for (int j = 0; j < mqttMessageLength; j++) { //Prints to Sim module
//      /   /GPRS.write(mqttMessage[j]); // Message contents
//     }/

//     GP/RS.write(byte(26)); // (signals end of message)

     
     Serial.println("Sent");
     delay(1000);

 //*********************************sending values to Mqtt********************/
    
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
     delay(10000);
     GPRS.println("AT+CIPCLOSE");
     Serial.println("AT+CIPCLOSE");
     delay(2000);
 
 }





boolean isGPRSReady(){

      
      GPRS.println("AT+CGATT=1");
      delay(1000);
      //UPDATE_SERIAL();
      
      GPRS.println("AT+CGATT?");
      int index = 0;
      char data1[15]={};
      String  gprsStr;


       while(GPRS.available()) 
      {
        data1[index++]=GPRS.read();//Forward what Software Serial received to Serial Port
       // Serial.write(gprsStr[index-1]);     
      }
      data1[index++] = '\0'; //end of character array
      gprsStr = data1;  //stores it in a string variable
     

      Serial.print("gprs str = ");
      Serial.println(data1);

         if (gprsStr.indexOf("+CGATT:1") > -1){
            Serial.println("GPRS OK");
            return true;
         }
         else {
            Serial.println("GPRS NOT OK");
            return false;
         }
         
}



// bool isGPRSReady(){

//    GPRS.println("AT");
//    delay(2000);
//    ShowSerialData();
   
//    //GPRS.println("AT");
//    GPRS.println("AT+CGATT?");
//    delay(2000);
//    ShowSerialData();
   
//    int index = 0;
//    char data1;
//    String gprsStr;

//     ShowSerialData();
    
//    while (GPRS.available()){
//       data1 = (char)GPRS.read();
//       Serial.write(data1);
//       gprsStr[index++] = data1;
//    }
    
//       Serial.println("Check OK");
//       Serial.print("gprs str = ");
//       Serial.println(gprsStr);
//       Serial.println(data1);
//       Serial.println("here\n");

//    if (data1>-1){
//       Serial.println("GPRS OK");
//    return true;
//    }
//    else {
//       Serial.println("GPRS NOT OK");
//    return false;
//    }

//    Serial.println("send AT to wake up GPRS");

// }
