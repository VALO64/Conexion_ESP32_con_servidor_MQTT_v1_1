/***************************************************************************
 *************************************************************************** 
 ** Conexion con un servidor MQTT utilizando una ESP32 Devkit V1_1        **  
 ** Oscar Alberto Valles Limas                                            **
 ** Oscar Beltran                                                         **
 ** Instituto tecnológico de Chihuahua                                    **
 ***************************************************************************
 ***************************************************************************
*/
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "/home/hero/Documents/Residencias/Conexion_ESP32_con_servidor_MQTT_v1_1/lib/ControlLed.h"

//WiFi Setup
const char* ssid = "IZZI-7E6C";     // WiFi ID
const char* password = "50A5DC507E6C";  // WiFi Password
 
 //MQTT Setup
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* clientID = "Prueba"; // Client ID username
const char* topic = "Itch1234"; // Publish topic

WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(clientID)) {
      Serial.println("MQTT connected");
      client.subscribe("/itch/labMeca/led/cmd/#");
      Serial.println("Topic Subscribed");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);  // wait 5sec and retry
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) { //Funcion callback
//-------------------------Entrada MQTT---------------------------------------------------  
  String valTpc = String(topic); //Se evalua el topico

  if(valTpc == "/itch/labMeca/led/cmd/on"){ //Si el topico es /itch/labMeca/led/cmd/on 
    Serial.println("/itch/labMeca/led/cmd/on"); //Se imprime el mensaje
    turnLed(1); //Enciende el led 
  }

  if(valTpc == "/itch/labMeca/led/cmd/off"){ //Se el topico es /itch/labMeca/led/cmd/off
    Serial.println("/itch/labMeca/led/cmd/off"); //Se imprime el mensaje 
    turnLed(0); //Se apaga el led 
  }

  String data = "";
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.println("");
  Serial.println("-----------------------");
}


void setup() {
  
  Serial.begin(115200);

  // Initialize device.
  setup_wifi();

  //Setting MQTT server
  client.setServer(mqttServer, mqttPort); 
  client.setCallback(callback); // Define function which will be called when a message is received.

  
  setup_();

}

void loop() {

  if (!client.connected()) { // If client is not connected
    reconnect(); // Try to reconnect
  }
  client.loop();
//-------------------------Entrada serial----------------------------------------------
  if (Serial.available() > 0) { //Si el puerto serial esta disponible
    String str = Serial.readString(); //Se lee una cadena de caracteres
    str.trim();
    Serial.println(str);

    if(str == "led=on") //Si la cadena de caracteres es led=on 
      turnLed(1); //Enciendo el led 

    if(str == "led=off") //Si la cadena de caracteres es led=off
      turnLed(0);  //El led se va a apgar 

  }
//-----------------------------Entrada del boton --------------------------------------
  evalBtn(); //Se invoca la funcion para evaluar el boton 
}