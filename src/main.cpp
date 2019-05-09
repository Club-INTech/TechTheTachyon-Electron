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
bool arrive = false;

void arriver(){
    arrive=true;
    digitalWrite(moteurs, LOW);
    ledcWrite(PWMChannel, 0);
    Serial.println("Contact");
}

void setup()
{
    //delay(5000);
    Serial.begin(9600);
    Serial.println("start");
    ledcSetup(PWMChannel, PWMfreq, PWMresolution);
    ledcAttachPin(led, PWMChannel);
    pinMode(moteurs, OUTPUT);
    digitalWrite(moteurs, LOW);
    pinMode(contacteur, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(contacteur), arriver, FALLING);
    ledcWrite(PWMChannel,100);
    Serial.println("init Wifi");

    WiFi.begin(ssid, pass);
    server.begin(serverPort);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Establishing connection to WiFi..");
    }
    Serial.println("Connecté avec l'adresse "+WiFi.localIP().toString());
    Serial.println("MAC:"+WiFi.macAddress());
    Serial.println("fin du setup");
    //delay(5000);
}

void loop()
{
    ledcWrite(PWMChannel, 255);
    //Serial.println(WiFi.status());
    digitalWrite(BLUE_LED, LOW);
    bool hadClient = client;
    while ( ! client) {
        client = server.available();
        delay(100);
    }
    if(!hadClient) {
        Serial.println("Connecté à "+client.remoteIP().toString());
    }
    digitalWrite(BLUE_LED, HIGH);
    Serial.println(client.available());
    if(client.available() > 0) {
        String data=client.readStringUntil('\n');
        if (data.equals("electron_launch")) {
            Serial.println("electron activated");
            client.print("@Belectron_activated\n");
        } else {
            Serial.println("Recu message inattendu: "+data);
        }
    }

    if (!moteuractive) {
        digitalWrite(moteurs, HIGH);
        moteuractive = true;
    }

    if (arrive) {
        ledcWrite(PWMChannel, 100);
        client.print("@Belectron_arrived\n");
    } else {
        ledcWrite(PWMChannel, 255);
    }
    delay(100);
}