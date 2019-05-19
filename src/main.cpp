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
const int PWMChannel = 1;
const int PWMresolution = 8;

//////////////MESSAGES//////////////////////////
String stopmessage="INTech Tech The Respect\n";
String runmessage="Que la force soit avec toi\n";
String arretedecrire="A";


///////////ETATS DE L'ELECTRON///////////////
bool moteuractive = false;
volatile bool arrive = false;
bool launched = false;

void arriver(){
    arrive=true;
    if(launched) { //Si on n'appuie pas sur le contacteur par erreur avant le début du match
        arrive = true;
        digitalWrite(moteurs, LOW);
        ledcWrite(PWMChannel, 0);
    }
}

void setup()
{
    pinMode(moteurs, OUTPUT);
    digitalWrite(moteurs, LOW);
    pinMode(contacteur, INPUT_PULLUP);

    Serial.begin(9600);
    Serial.println("start");
    ledcSetup(PWMChannel, PWMfreq, PWMresolution);
    ledcAttachPin(led, PWMChannel);
    attachInterrupt(digitalPinToInterrupt(contacteur), arriver, FALLING);
    ledcWrite(PWMChannel,100);
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
    ledcWrite(PWMChannel, 255);
    //Serial.println(WiFi.status());
    digitalWrite(BLUE_LED, HIGH);
    if(!client) {
        client = server.available();
    }
    if (client.connected()) {
        if(arrive) {
            client.print("@Belectron_arrived\n");
        }

        Serial.println("Connecté à "+client.remoteIP().toString());
        if(client.available()) {
            String data=client.readStringUntil('\n');
            if (data.equals("electron_launch")) {
                Serial.println("electron activated");
                client.print("@Belectron_activated\n");
                launched = true;
            } else {
                Serial.println("Recu message inattendu: "+data);
            }
        }
    }

    if(launched) {
        if(!arrive) {
            digitalWrite(moteurs, HIGH);
        } else {
            digitalWrite(moteurs, LOW);
        }
    }

    if (arrive) {
        ledcWrite(PWMChannel, 100);
    } else {
        ledcWrite(PWMChannel, 255);
    }
}