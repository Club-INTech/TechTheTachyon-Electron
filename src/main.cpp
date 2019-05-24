#include "Arduino.h"
#include <WiFi.h>
#include <SPI.h>


/////////////////CONFIG WIFI///////////////////////
// Set your Static IP address
IPAddress local_IP(192, 168, 42, 69);
// Set your Gateway IP address
IPAddress gateway(192, 168, 42, 69);

IPAddress subnet(255, 255, 255, 0);
const char ssid[] = "intech_electron_ap";
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

///////////Etats de la led/////////////////////
///                                         ///
///       Connexion au WiFi 0.5             ///
///                          \/             ///
///  Connection d'un client 1.0             ///
///                          \/             ///
/// Activation du moteur clignotement       ///
///                          \/             ///
///         Electron arrivé 0.0             ///
///                                         ///
///////////////////////////////////////////////

///////////ETATS DE L'ELECTRON///////////////
volatile bool arrive = false;
long lastBlinkTime=0;
bool blink=false;
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

void clignote(){
    if(millis()-lastBlinkTime>500){
        if(blink) {
            ledcWrite(ledChannel, 0);
        } else{
            ledcWrite(ledChannel, 255);
        }
        blink=!blink;
        lastBlinkTime=millis();
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
    ledcWrite(motorChannel, 0);
    Serial.println("init Wifi");

    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, pass);
   /* while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Establishing connection to WiFi..");
    }*/
    server.begin();
    ledcWrite(ledChannel,100);
    Serial.println("Connecté avec l'adresse "+WiFi.localIP().toString());
    Serial.println("MAC:"+WiFi.macAddress());
    Serial.println("fin du setup");
    //delay(5000);
}

void loop()
{
    //Serial.println(WiFi.status());
    digitalWrite(BLUE_LED, HIGH);
    boolean hadClient=client;
    if(!client) {
        client = server.available();
    }

    if(!hadClient) {
        const IPAddress &ip = client.remoteIP();
        if(ip[0] == 192) {
            Serial.println("Client à l'adresse: " + ip.toString());
            client.println("Bienvenue à toi, l'ami");
            ledcWrite(ledChannel, 255);
        }
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
        clignote();
    }

    //Serial.println("MotorValue: "+(String)motorValue);

    if (arrive) {
        client.print("@Belectron_arrived\n");
    }
}