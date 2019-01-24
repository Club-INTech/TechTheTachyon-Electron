#include "Arduino.h"
#include <WiFi.h>
#include <SPI.h>

char ssid[] = "INTech2";
char pass[] = "$0P@L1Z7";
int status = WL_IDLE_STATUS;
WiFiServer server(1296);
IPAddress ip;
WiFiClient client;
int moteurs = 22;
int contacteur = 23;
bool test= true;
String stopmessage="INTech Tech The Respect\n";
String runmessage="Que la force soit avec toi\n";
String arretedecrire="A";
String message;
bool attente = true;
String demarre="demarre";


void setup() {
    delay(5000);
    Serial.begin(9600);
    pinMode(moteurs,OUTPUT);
    digitalWrite(moteurs,LOW);

    status =WiFi.begin(ssid, pass);
    while ( status != WL_CONNECTED ) {
        Serial.println("ESP32 se connecte...");
        status = WiFi.status();
        Serial.println(status);
        delay(100);
    }
    server.begin();
    Serial.print("Connected to wifi. My address : ");
    ip = WiFi.localIP();
    Serial.println(ip);
}

void loop() {

    while (attente){
        if(Serial.readString()==demarre){
            attente=false;
        }
    }

    if (client.connect("192.168.4.1",1296)){
        while (true) {
            if (test) {
                client.print(runmessage);
                Serial.println(runmessage);
                delay(5000);
            }
            if (client.available()) {
                delay(500);
                message = client.readString();
                Serial.println(message);
                if (message == arretedecrire){
                    Serial.println("arrive");
                    while (true){

                    }
                }
            }
        }
    }
    delay(500);
}
