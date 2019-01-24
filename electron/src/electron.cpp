#include "Arduino.h"
#include <WiFi.h>
#include <SPI.h>

char ssid[] = "INTech2";
char pass[] = "$0P@L1Z7";
int status = WL_IDLE_STATUS;
WiFiServer server(1296);
WiFiClient client;
int moteurs = 22;
int contacteur = 23;
String stopmessage="INTech Tech The Respect\n";
String runmessage="Que la force soit avec toi\n";
String arretedecrire="A";
bool test = true;


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

