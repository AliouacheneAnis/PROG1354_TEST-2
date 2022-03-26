/*
  Titre      : I2C Protocol
  Auteur     : Anis Aliouachene
  Date       : 15/03/2022
  Description: Utilisation de deux peripherique d'entre : BME280 et RCT et une de sortie : OLED avec le protocol I2C et le ESP32
  BME 280 nous permet de capter temperature et humidity et RTC pour afficher l'heure et la date 
  OLED sert a afficher les information chaque 5 min et tous les composantes se communique via le bus I2C 
  Version    : 0.0.2 
*/


/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-i2c-communication-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Inclus les libraires necessaires 
#include <Arduino.h>
#include <Wire.h>  // Wire pour Prototcol I2C
#include <Adafruit_Sensor.h>   // Pour les capteur et Sensor adafruit 
#include <Adafruit_GFX.h>   // Pour fonctionner l'ecran OLED 
#include<WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include "Fonction.hpp"
#include "Secret.hpp"
#include "page.hpp"


#define I2C_SDA 21   // define la broche SDA a utiliser sur ESP32 
#define I2C_SCL 22   // define la broche SCL a utiliser sur ESP32 


// Declarations d'objets
TwoWire I2CBME = TwoWire(0);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


// Fonction Setup 
void setup() {

    Serial.begin(9600); // Debut moniteur serie sur la frequence 9600 
    Serial.println(F("BME280 test"));

    // Debut de la communication I2C avec le bme280 
    I2CBME.begin(I2C_SDA, I2C_SCL, 100000);
    
    bool status;

    status = bme.begin(0x76, &I2CBME);   // status de la communication I2C avec le bme280 
    if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
    }

    
    // Debut de la communication I2C avec l'ecran sur l'adresse 0x3C
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }

    // Debur de la communication I2C avec RTC 
    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Serial.flush();
      while (1) ;
    }

    Serial.println("-- Default Test --");
    Serial.println();

    display.clearDisplay();
    display.setTextColor(WHITE);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/html", index_html);
    });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", ReadTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", ReadHumidity().c_str());
  });

   server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", ReadPressure().c_str());
  });

  server.on("/time", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", TimeRead().c_str());
  });

    AsyncElegantOTA.begin(&server);    // Start ElegantOTA
    server.begin();
    Serial.println("HTTP server started");

}

void loop() { 

  // Appel\la fonction popur afficher la temperature, humidity et pression sur l'ecran OLED 
  displayTmpHum();

  // Appel\la fonction popur afficher le temps sur l'ecran OLED 
  displayTime();
}


