#include "Arduino.h"
#include <WiFi.h>
#include <SPI.h>


/////////////////CONFIG WIFI///////////////////////
const char ssid[] = "nucsamere";
//const char pass[] = "$0P@L1Z7";
const char pass[]= "suussuus";
//const byte serverAdress[] = {192, 168, 12, 247};
const int serverPort=18900;
WiFiClient client;
WiFiServer server(serverPort);


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
    while ( ! client) {
        //Serial.println("attente client");
        client = server.available();
        delay(100);
    }
    Serial.println("Connecté à "+client.remoteIP().toString());
    delay(100);
    Serial.println(client.available());
    if(client.available()>0) {
        String data=client.readStringUntil('\n');
        if (data.equals("electron_launch")) {
            Serial.println("electron activated");
            ledcWrite(PWMChannel, 255);
            delay(200);
            ledcWrite(PWMChannel, 100);
            if (!moteuractive) {
                digitalWrite(moteurs, HIGH);
                moteuractive = true;
            }
            client.print("@Belectron_activated\n");
        }else{
            Serial.println("Recu message inattendu: "+data);
        }
    } else{
        Serial.println("Rien recu");
    }

    if (arrive) {
        client.print("@Belectron_arrived\n");

    }
}