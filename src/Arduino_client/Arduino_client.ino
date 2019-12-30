 
#include "conMqttPass.h"
#include "publish_function.h"

char MQTT_HOST_DOMAIN[] = "farmer.cloudmqtt.com"; //brokerUrl
char MQTT_TCP_PORT[] = "10330"; //brokerPort
char MQTT_WEBSOCKET_PORT[] = "30330";
char MQTT_TOPIC[] = "hive/light_status";
//#define GSM_ON 7
char CLIENT_ID[] = "testClientID";

char USERNAME[] = "jxyowems"; //username
char PASSWORD[] = "zxvG7oUXAvf8"; //password

  
 void setup(){
     serialComs(9600,19200,20000);//void serialComs(int baud_Serial_monitor, int baud_sim_module, int delay_micro)
     Serial.println("Starting");
 }



 void loop(){

  //     digitalWrite(GSM_ON, HIGH); //Starts the SIM module.
       delay(5000); //wait for 20 seconds GPRS getting stable

      boolean gprsReady = isGPRSReady();

       
       if(gprsReady){
          Serial.println("Ready to send");
          Serial.println("Sendind light: \"on\"");
          
         char light_status[] = "on";
           /* The arguments here are:
        clientID, IP, Port, Topic, Message, Username, Password*/
          sendMQTTMessage(CLIENT_ID, MQTT_HOST_DOMAIN, MQTT_TCP_PORT, MQTT_TOPIC,light_status,USERNAME,PASSWORD); 
       }

 }
