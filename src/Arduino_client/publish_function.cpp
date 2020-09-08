#include <Arduino.h>
#include <SoftwareSerial.h>
#include "conMqttPass.h"
#include "publish_function.h"

SoftwareSerial GPRS(3,2); //to TX_A6, to RX_A6
byte mqttMessage[127];




void serialComs(int baud_Serial_monitor, int baud_sim_module, int delay_micro){ //initial serial commnication
   
     Serial.begin(baud_Serial_monitor);
     GPRS.begin(baud_sim_module);
     delay(delay_micro);

}





void setApn(){
     boolean apn_off = true; //setting APN     
     while (apn_off){
       String gprsApn;
       GPRS.println("AT+CSTT=\"gloflat\",\"flat\",\"flat\""); // Puts phone into GPRS mode  AT+ CSTT=<apn>, <user id>, <password>
       delay(5000); // Wait 5 second
       if( GPRS.available() ) { 
          gprsApn = GPRS.readString();
          if ( gprsApn.indexOf("OK")  > -1 ) {apn_off = false; Serial.println("GPRS APN SET");}
       }
     } //setting APN 
}






void connectWireless(){
     boolean conwire_off = true; //setting wireless connection     
     while (conwire_off){
       String gprsWireless;
        GPRS.println("AT+CIICR"); //Bring up wireless connection with GPRS
        delay(2000); // Wait 5 second
        if( GPRS.available() ) { 
          gprsWireless = GPRS.readString();
          if ( gprsWireless.indexOf("ERROR")  == -1 ) {conwire_off = false; Serial.println("WIRELESS CONNECTION SET");}
          else {Serial.println(gprsWireless);}
       }
     } //setting wireless connection
}






void selectNetwork(){  //AT+COPS=0
     boolean auto_off = true; //setting wireless connection     
     while (auto_off){
       String auto_sel;
        GPRS.println("AT+COPS=0"); //Bring up wireless connection with GPRS
        delay(2000); // Wait 5 second
        GPRS.println("AT+COPS?"); //* AT+COPS? – List the network currently registered on
        delay(1000);
        if( GPRS.available() ) { 
          auto_sel = GPRS.readString();
          if ( auto_sel.indexOf("ERROR")  == -1 ) {auto_off = false; Serial.println("NETWORK SELECTED");}
       }
     } //setting wireless connection
}






String getIpAddress(){ //gets IP address GPRS.println("AT+CIFSR");

  boolean ip_address_not_gotten = true;
  String ip_address_state;
    while(ip_address_not_gotten){
       GPRS.println("AT+CIFSR"); //get local IP Address
       delay(2000);
       if(GPRS.available()) { 
        ip_address_state = GPRS.readString();
          if (ip_address_state.indexOf("ERROR") == -1) { ip_address_not_gotten = false; }  
          else{Serial.println("ERROR: "+ip_address_state);} 
       delay(2000);
      }
   }
 return ip_address_state;
}






void establishTcpConnection( char * broker_url, char * broker_port ){ //Sets up TCP/IP connection to MQTT Broker (essentially a server)

  char atCommand[50];
  boolean is_tcp_not_established = true;
  String tcp_connect_state;

   while(is_tcp_not_established){
     strcpy(atCommand, "AT+CIPSTART=\"TCP\",\"");
     strcat(atCommand, broker_url);
     strcat(atCommand, "\",");
     strcat(atCommand, broker_port);
     //strcat(atCommand, "\""); //example from A6 manual text AT+CIPSTART="TCP","124.42.0.80",7

 
     GPRS.println(atCommand);
     delay(2000);  //gives UART peripheral enough time to transmit the atCommand message.
     
     if( GPRS.available()) {tcp_connect_state = GPRS.readString();}
     delay(50);

     if (tcp_connect_state.indexOf("ERROR") == -1) { //if the ERROR is not returned, 
        is_tcp_not_established = false;  Serial.println("SERVER CONNECTED");
      }
      else { Serial.println(tcp_connect_state); }
      
   }
} //end of function: void establishTcpConnection( char * broker_url, char * broker_port )





void initiateMqtt(char* client_id, char* user_name, char* pass_word ){ 
     
     boolean is_initiate_mqtt_not_successful = true;
     String initiate_mqtt_state;

     int mqttMessageLength = 20 + strlen(client_id) + strlen(user_name) + strlen(pass_word);
     Serial.println(mqttMessageLength);
     
     mqtt_connect_message(mqttMessage, client_id, user_name, pass_word); //Prepares the MQTT connect message
    
    while(is_initiate_mqtt_not_successful){ 

       GPRS.println("AT+CIPSEND");
       delay(2000);
        
       for (int j = 0; j < mqttMessageLength; j++) { GPRS.write(mqttMessage[j]); delay(50);} //sends message
       GPRS.write(byte(26)); // (signals end of message)
       delay(2000); //allow uart peripheral to finish up

       if (GPRS.available()) { initiate_mqtt_state = GPRS.readString();
              if (initiate_mqtt_state.indexOf("ERROR") == -1) {
                is_initiate_mqtt_not_successful = false;}
              else { Serial.println(initiate_mqtt_state);}
       }
       
   }
    
     Serial.println("\n--------------MQTT_CONNECT_MESSAGE_SENT--------------");
     delay(5000);   

}



void sendMqttMessage( char* topi_c, char* messag_e){ 

      
     boolean is_mqtt_message_not_successful = true;
     String mqtt_message_state;
     int mqttMessageLength = 4 + strlen(topi_c) + strlen(messag_e);
     mqtt_publish_message(mqttMessage, topi_c, messag_e);
     Serial.println(mqttMessageLength);
     
     mqtt_publish_message(mqttMessage, topi_c, messag_e);

    while(is_mqtt_message_not_successful){ 

     GPRS.println("AT+CIPSEND");
       delay(2000);
        
       for (int k = 0; k < mqttMessageLength; k++) { GPRS.write(mqttMessage[k]); delay(50);} //sends message
       GPRS.write(byte(26)); // (signals end of message)
       delay(2000); //allow uart peripheral to finish up

       if (GPRS.available()) { mqtt_message_state = GPRS.readString();
              if (mqtt_message_state.indexOf("ERROR") == -1) {
                is_mqtt_message_not_successful = false;}
              else { Serial.println(mqtt_message_state);}
       }
       
   }
     
}


void closeTcpConnection(){

    Serial.println("-------------MQTT_MESSAGE_SENT-------------"); // 
     delay(9000);
     GPRS.println("AT+CIPCLOSE");
     if (GPRS.available()) {Serial.println(GPRS.readString());}
     delay(2000);
 
}


void sendMQTTMessage(char* clientId, char* brokerUrl, char* brokerPort, char* topic, char* message, char* username, char* password, int state){

  if (state<1){
     selectNetwork();
     setApn(); //Connect sim to packet Domain name server
     connectWireless(); //
     Serial.println(getIpAddress()); //gets IP address;
  } 
  
     establishTcpConnection(brokerUrl, brokerPort);  //establishes TCP/IP connection to Broker
     initiateMqtt(clientId, username, password ); //sends MQTT connect message to broker
     sendMqttMessage(topic, message); //Sends message to mqtt broker on paticular topic
     closeTcpConnection(); //ends TCP/IP connection to broker     
}


boolean isGPRSReady(){

      //int index = 0;
      String  gprsStr;

     GPRS.println("AT"); // Sends AT command to wake up cell phone
     delay(2000); // Wait a second
     if( GPRS.available() ) Serial.println(GPRS.readString()  + "awoken \n");
     delay(2000);


      GPRS.println("AT+CGATT=1");
      delay(100); //delay by 0.1 seconds
      //if(GPRS.available()) Serial.println(GPRS.readString()); 


      GPRS.println("AT+CGATT?");
      //if(GPRS.available) Serial.println(GPRS.readString());

      if(GPRS.available()) { gprsStr = GPRS.readString(); } 

      Serial.write("gprs str = ");
      Serial.println(gprsStr);

         if (gprsStr.indexOf("+CGATT:1") > -1){
            Serial.println("GPRS OK");
            return true;
         }
         else {
            Serial.println("GPRS NOT OK");
            return false;
         }
         
}
