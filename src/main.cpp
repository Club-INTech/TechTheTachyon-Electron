#include "Arduino.h"
#include <WiFi.h>
#include <SPI.h>


/////////////////CONFIG WIFI///////////////////////
const char ssid[] = "nucsamere";
//const char pass[] = "$0P@L1Z7";
const char pass[]= "suussuus";
const byte serverAdress[] = {192, 168, 12, 1};
const int serverPort=80;
WiFiClient client;


///////////////////GPIO///////////////////////////
int moteurs = 13;
int contacteur = 32;
int led = 33;
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
    /*
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid,pass);
    WiFi.softAPConfig(IPAddress(192,168,5,1),IPAddress(192,168,5,1),IPAddress(255,255,255,0));
    */
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Establishing connection to WiFi..");
    }
    Serial.println("Connecté avec l'adresse "+WiFi.localIP().toString());

    Serial.println(client.connect(serverAdress, serverPort));
    Serial.println("fin du setup");
    //delay(5000);
}

void loop()
{
    ledcWrite(PWMChannel, 255);
    //Serial.println(WiFi.status());
    while (WiFi.status()!=WL_CONNECTED ){//&& !client.connect(serverAdress,serverPort)){
        Serial.println("attente connection serveur");
        delay(10);
    }
    client.println("ready");
    if(client.available()) {
        if (client.readString() == "electron_launch") {
            ledcWrite(PWMChannel, 255);
            delay(200);
            ledcWrite(PWMChannel, 100);
            if (!moteuractive) {
                digitalWrite(moteurs, HIGH);
                moteuractive = true;
            }
            client.print("@Belectron_activated\n");
        }
    }
    if (arrive) {
        client.print("@Belectron_arrived\n");
    }
}