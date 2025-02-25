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
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <PubSubClient.h>
#include <ControlLed.h> //Libreria para el control de los leds 

 //MQTT Setup
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* clientID = "Prueba"; // Client ID username
const char* topic = "Itch1234"; // Publish topic

WiFiClient espClient;
PubSubClient client(espClient);

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

  WiFiManager wifiManager;
  wifiManager.resetSettings(); //Esto solo es para modo de prueba, en el codigo final, hay que comentar
  //first parameter is name of access point, second is the password
  wifiManager.autoConnect("ESP32_Prueba", "Itch1234"); //Nombre del hotspot que se va a crear y su contrasena
  Serial.println("Ya estas conectado"); //Impresion de mensaje
  //Setting MQTT server
  client.setServer(mqttServer, mqttPort); 
  client.setCallback(callback); // Define function which will be called when a message is received.
  setup_(); //Invoca a setup_ que se encuentra en la libreria ControlLed.h
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