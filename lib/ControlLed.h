#include <Arduino.h>

bool rebote = 0; //Variable para el anti rebote  
bool togle = 0; //Variable para sujetar 

int led = 5; //Led en el pin 5 de la ESP32
int btn = 21; //Boton en el pin 21 de la ESP32

void setup_() {

  pinMode(led, OUTPUT); //Led configurado como salida
  pinMode(btn, INPUT); //Boton configurado como entrada 
}

void turnLed(int v) { //Funcion para encender el led va a recibir un valor entero v 

  if(v == 1)  digitalWrite(led, 1); //Si v es igual a 1 el led se va a encender 
  else        digitalWrite(led, 0); //Si el v no es igual a 1 el led se va a apagar 
}

void evalBtn() { //Funcion para evaluar el estado del boton 
  
  int valBtn = digitalRead(btn); //Evaluamos el estado del boton 


  if(valBtn == 1 && !rebote) //Si el boton es presionado y ademas el valor del antirebote es 1
  {
    rebote = true; //El rebote se va a volver verdadero 
    Serial.println("On"); //Se imprime el mensaje de enecendido 

    if(togle == 0) //No se va a enganchar 
      turnLed(1); //Se enciende el led
    else
      turnLed(0); //En caso contrario el led se va a pagar 

    togle = !togle;  //El enganche vuevle a su valor inicial 
  }  
  else if(valBtn == 0 && rebote) { //Pero si el valor del boton es igual a cero o no esta presionado 
    rebote = false; //No hay rebote 
    Serial.println("Off"); //Se muestra un mensaje de apagado 
  }

  delay(100); // Retraso de 100 milisegundos 
}
