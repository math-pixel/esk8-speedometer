
// BRANCHEMENT
/*
arduino | hc-05
----------------
D3       | tx
D6       | rx
----------------
        | screen
A4      | SDA
A5      | SCK
*/


// lib ecran
#include <Adafruit_SH1106.h>

#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <SPI.h>
#include <Wire.h>

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

#include <SoftwareSerial.h>


// Création de l'objet bt pour la communication bluetooth
SoftwareSerial BT(3, 6); //rx, tx => brancher en inverse sur le module 

byte PinStateBT = 5;
boolean BTconnected = false;

// variable recevant les données transmises par votre tablette
char reception_des_donnees ;

// variable recevant les chaines de caracteres
String chaine_caractere ;
int valeur ;


void setup() {
  
  // ------------ INIT VAR
  pinMode(PinStateBT, INPUT);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);

  // ------------ INIT BT
  BT.begin(9600);

  // ------------ display splash screen
  display.display();
  delay(2000);
  display.clearDisplay();

  // ------------ wait until the HC-05 has made a connection

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Waiting");
  display.println("Bluetooth");
  display.println("Connection ...");
  display.display();
  delay(2000);
  display.clearDisplay();
  
    while (!BTconnected)
    {
      if ( digitalRead(PinStateBT)==HIGH)  { BTconnected = true;};
    }

  // ------------ BT COnnected !
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println("Bluetooth");
  display.println("Connection");
  display.println("=> OK !");
  display.display();
  delay(4000);
  display.clearDisplay();
}


String vitesse = "Null";
boolean needRefreshSpeed = true;
boolean refreshDisconnection = true;
void loop() {


  if(isConnected_Ble()){
    processBluetoothConnected();
    refreshDisconnection = true;
  }else{
    processBluetoothNotConnected();
  } 

}


void processBluetoothConnected(){
  // refresh screen
  if(needRefreshSpeed == true){
    Serial.println("refresh");
    needRefreshSpeed = false;

    display.setTextSize(1.5);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("Vitesse : ");
    display.setTextSize(3);
    appendTextCenter(vitesse);
    display.display();
    display.clearDisplay();
  }

  chaine_caractere = ""; 
  // available() permet d'obtenir le nombre de caractères  disponibles pour la
  // lecture à partir d'un port série logiciel. Il s'agit de données déjà arrivées
  // et stockées dans le tampon de réception série.
  while (BT.available() != 0) {
   
    //lecture du buffer
    reception_des_donnees = BT.read();

    // chaine_caractere = reception_des_donnees;
     chaine_caractere.concat(reception_des_donnees);
     delay(20);
  }
  
    if (chaine_caractere != "")
    {
      Serial.println(chaine_caractere);
      vitesse = chaine_caractere;
      needRefreshSpeed = true;
    }
}


void processBluetoothNotConnected(){
  if(refreshDisconnection == true){
    refreshDisconnection = false;
    
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0,0);
    appendTextCenter("Bluetooth \n Disconnected");
    display.display();
    display.clearDisplay();
  }
}

// TOOLS FUNCTION


boolean isConnected_Ble(){
  return digitalRead(PinStateBT);
}


void appendTextCenter(String text) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

  display.setCursor((128 - width) / 2, (64 - height) / 2);
  display.println(text); // text to display
}
