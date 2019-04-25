#include "Arduino.h"
#include <WiFi.h>
#include <SPI.h>


/////////////////CONFIG WIFI///////////////////////
char ssid[] = "INTech2";
char pass[] = "$0P@L1Z7";
int status = WL_IDLE_STATUS;
WiFiServer server;
WiFiClient client;


///////////////////GPIO///////////////////////////
int moteurs = 13;
int contacteur = 23;
int led = 35;
const int PWMfreq = 5000;
const int PWMChannel = 0;
const int PWMresolution = 8;

//////////////MESSAGES//////////////////////////
String stopmessage="INTech Tech The Respect\n";
String runmessage="Que la force soit avec toi\n";
String arretedecrire="A";


///////////ETATS DE L'ELECTRON///////////////
bool moteuractive = false;
bool arrive = false;


void setup()
{
    //delay(5000);
    Serial.begin(9600);
    Serial.println("start");
    ledcSetup(PWMChannel, PWMfreq, PWMresolution);
    pinMode(led,OUTPUT);
    ledcAttachPin(led, PWMChannel);
    pinMode(moteurs, OUTPUT);
    digitalWrite(moteurs, LOW);
    pinMode(contacteur, INPUT_PULLUP);
    ledcWrite(led,127);
    Serial.println("init Wifi");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid,pass);
    delay(100);
    WiFi.softAPConfig(IPAddress(192,168,5,1),IPAddress(192,168,5,1),IPAddress(255,255,255,0));

    server.begin(18900);
    Serial.println("fin du setup");
    //delay(5000);
}

void loop()
{
    ledcWrite(led, 255);
    while (!client) {
        client = server.available();
        delay(10);
        if (digitalRead(contacteur) == LOW)
        {
            Serial.println("Contact!");
            digitalWrite(moteurs,LOW);
            arrive = true;
        }

    }ledcWrite(led, 0);
    if(client.available()) {
        if (client.readString() == "electron_launch") {
            ledcWrite(led, 255);
            delay(200);
            ledcWrite(led, 0);
            if (!moteuractive) {
                digitalWrite(moteurs, HIGH);
                moteuractive = true;
            }
            client.print("@Belectron_activated\n");


        }
    }
    if (digitalRead(contacteur) == LOW)
    {
        Serial.println("Contact!");
        digitalWrite(moteurs,LOW);
        ledcWrite(led, 127);
        arrive = true;
    }
    if (arrive) {
        client.print("@Belectron_arrived\n");
    }
}

/*
void setup() {
    delay(5000);
    Serial.begin(9600);
    pinMode(moteurs,OUTPUT);
    digitalWrite(moteurs,LOW);
    pinMode(contacteur, INPUT_PULLDOWN);

    WiFi.softAP(ssid, pass);
    IPAddress IP = WiFi.softAPIP();
    server.begin();
    Serial.print("IP du serveur : ");
    Serial.println(IP);
}

void loop() {
    client = server.available();
    if ( client ) {
        while (true) {
            if (digitalRead(contacteur) == LOW) {
                delay(100);
                if (digitalRead(contacteur) == LOW) {
                    digitalWrite(moteurs, LOW);
                    Serial.println("Vous ne passerez pas");
                    while (true) {
                        delay(1000);
                        Serial.println("Connecté au copain");
                        client.print(arretedecrire);
                        Serial.println(arretedecrire);
                        delay(4000);
                    }
                }
            }
            if (client.connected() & test) {
                Serial.println("client send a message");
                delay(100)
;                String message = client.readString();
                if (message == runmessage) {
                    Serial.println("Vers l'infini et au delà");
                    digitalWrite(moteurs, HIGH);
                    test=false;
                } else if (message == stopmessage) {
                    Serial.println("STOP");
                    digitalWrite(moteurs, LOW);
                } else {
                    Serial.println("j'ai reçu du caca");
                }
            }
        }
    }

}
 */