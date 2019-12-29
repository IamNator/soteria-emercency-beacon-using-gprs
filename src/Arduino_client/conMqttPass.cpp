#include "conMqttPass.h"
//Using password.

void mqtt_connect_message(uint8_t * mqtt_message, char * client_id, char * username,  char * password ) {

        uint8_t i = 0;
        uint8_t client_id_length = strlen(client_id);
        uint8_t username_length = strlen(username);
        uint8_t password_length = strlen(password);

		mqtt_message[0] = 16;                      // MQTT Message Type CONNECT   0x10
		mqtt_message[1] = 14 + client_id_length;   // Remaining length of the message  

		mqtt_message[2] = 0;                       // Protocol Name Length MSB
		mqtt_message[3] = 6;                       // Protocol Name Length LSB
		mqtt_message[4] = 77;                      // ASCII Code for M
		mqtt_message[5] = 81;                      // ASCII Code for Q
		mqtt_message[6] = 73;                      // ASCII Code for I
		mqtt_message[7] = 115;                     // ASCII Code for s
		mqtt_message[8] = 100;                     // ASCII Code for d
		mqtt_message[9] = 112;                     // ASCII Code for p
		mqtt_message[10] = 3;                      // MQTT Protocol version = 3
		mqtt_message[11] = 2;                      // conn flags
		mqtt_message[12] = 0;                      // Keep-alive Time Length MSB
		mqtt_message[13] = 15;                     // Keep-alive Time Length LSB


		mqtt_message[14] = 0;                      // Client ID length MSB
		mqtt_message[15] = client_id_length;       // Client ID length LSB

        // Client ID
        for(i = 0; i < client_id_length + 16; i++){
            mqtt_message[16 + i] = client_id[i];
        }

        mqtt_message[17+i] = 0;                      // ULEN MSB
		mqtt_message[18+i] = username_length;        // username Length LSB

        uint8_t addy = 19 + i;                       //next charater's index
         // Username
        for(i = 0; i < username_length + addy ; i++){
            mqtt_message[addy + i] = username[i];
        }

        addy = addy + i;
        mqtt_message[addy + 1] = 0;                    // PWLEN MSB
		mqtt_message[addy + 2] = password_length;      // password Length LSB

        addy = addy + 3;                               //next charater's index
         // Username
        for(i = 0; i < password_length + addy ; i++){
            mqtt_message[addy + i] = password[i];
        }

//an example
//CONN	RL	PLEN		M	Q	I	s	d	p   	LVL	FL	KA		CIDLEN		A	B	C	D	E	F	 ULEN		d	x	x	k	g	k	p	p	    PWLEN		q	A	U	Z	B	d	a	S	I	U	L	x
//10	2C	0006	   4D   51	49	73	64	70  	03	C2	003C	00	06	    41	42	43	44	45	46	 0008	    64	78	78	6B	67	6B	70	70  	000C	    71	41	55	5A	42	64	61	53	49	55	4C	78


}



void mqtt_publish_message(uint8_t * mqtt_message, char * topic, char * message) {

        uint8_t i = 0;
        uint8_t topic_length = strlen(topic);
        uint8_t message_length = strlen(message);

		mqtt_message[0] = 48;                                  // MQTT Message Type CONNECT
		mqtt_message[1] = 2 + topic_length + message_length;   // Remaining length
		mqtt_message[2] = 0;                                   // MQTT Message Type CONNECT
		mqtt_message[3] = topic_length;                        // MQTT Message Type CONNECT

        // Topic
        for(i = 0; i < topic_length; i++){
            mqtt_message[4 + i] = topic[i];
        }

        // Message
        for(i = 0; i < message_length; i++){
            mqtt_message[4 + topic_length + i] = message[i];
        }
      *(arr+i) = 'h'; //same as arr[i] = 'h';

	}

	void mqtt_disconnect_message(uint8_t * mqtt_message) {
		mqtt_message[0] = 0xE0; // msgtype = connect
		mqtt_message[1] = 0x00; // length of message (?)
}
