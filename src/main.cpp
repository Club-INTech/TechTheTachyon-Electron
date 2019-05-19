#include "Arduino.h"
#include <WiFi.h>
#include <SPI.h>


/////////////////CONFIG WIFI///////////////////////
const char ssid[] = "nucsamere";
const char pass[]= "suussuus";
const int serverPort=18900;
WiFiClient client;
WiFiServer server(serverPort);


///////////////////GPIO///////////////////////////
uint8_t moteurs = 12;
uint8_t contacteur = 32;
uint8_t led = 33;
uint8_t BLUE_LED = 2;
const int PWMfreq = 1000;
const int ledChannel = 1;
const int motorChannel = 2;
const int PWMresolution = 8;
int motorValue=0;

//////////////MESSAGES//////////////////////////
String stopmessage="INTech Tech The Respect\n";
String runmessage="Que la force soit avec toi\n";
String arretedecrire="A";


///////////ETATS DE L'ELECTRON///////////////
volatile bool arrive = false;
bool launched = false;

void arriver(){

    Serial.println("Contact");

    if(launched) { //Si on n'appuie pas sur le contacteur par erreur avant le début du match
        arrive = true;
        ledcWrite(motorChannel, 0);
        //digitalWrite(moteurs, LOW);
        ledcWrite(ledChannel, 0);
    }
}

void setup()
{
    pinMode(moteurs, OUTPUT);
    digitalWrite(moteurs, LOW);
    pinMode(contacteur, INPUT_PULLUP);

    Serial.begin(9600);
    Serial.println("start");
    ledcSetup(ledChannel, PWMfreq, PWMresolution);
    ledcSetup(motorChannel, PWMfreq, PWMresolution);
    ledcAttachPin(led, ledChannel);
    ledcAttachPin(moteurs, motorChannel);
    //pinMode(moteurs, OUTPUT);
    //digitalWrite(moteurs, LOW);
    pinMode(contacteur, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(contacteur), arriver, FALLING);
    ledcWrite(ledChannel,100);
    ledcWrite(motorChannel, 0);
    Serial.println("init Wifi");

    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Establishing connection to WiFi..");
    }
    server.begin();
    Serial.println("Connecté avec l'adresse "+WiFi.localIP().toString());
    Serial.println("MAC:"+WiFi.macAddress());
    Serial.println("fin du setup");
    //delay(5000);
}

void loop()
{
    ledcWrite(ledChannel, 255);
    //Serial.println(WiFi.status());
    digitalWrite(BLUE_LED, HIGH);
    boolean hadClient=client;
    if(!client) {
        client = server.available();
    }

    if(!hadClient) {
        Serial.println("Client à l'dresse: "+client.remoteIP().toString());
        client.println("Bienvenue à toi, l'ami");
    }
    digitalWrite(BLUE_LED, HIGH);
    int toRead=client.available();
    if(toRead > 0) {
        Serial.println("Nb d'octets à lire: " + (String) (toRead));
        String data = client.readStringUntil('\n');
        if (data.equals("electron_launch")) {
            Serial.println("electron activated");
            client.print("@Belectron_activated\n");
            launched = true;
        } else {
            Serial.println("Recu message inattendu: " + data);
        }
    }

    //*
    if(digitalRead(contacteur)==LOW){
        arriver();
    }
    //*/

    if(launched && !arrive) {
        if(motorValue<255){
            motorValue+=10;
        }else{
            motorValue=255;
        }
        ledcWrite(motorChannel, motorValue);
    }

    Serial.println("MotorValue: "+(String)motorValue);

    if (arrive) {
        ledcWrite(ledChannel, 100);
        client.print("@Belectron_arrived\n");
    } else {
        if (launched) {
            ledcWrite(ledChannel, 255);
        }
    }
}