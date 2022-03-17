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
#include <Adafruit_BME280.h> // Pour fonctionner le BME280
#include <Adafruit_GFX.h>   // Pour fonctionner l'ecran OLED 
#include <Adafruit_SSD1306.h> //  Pour fonctionner l'ecran OLED 
#include "RTClib.h" // Pour fonctionner le RTC 



#define I2C_SDA 21   // define la broche SDA a utiliser sur ESP32 p
#define I2C_SCL 22   // define la broche SCL a utiliser sur ESP32 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SEALEVELPRESSURE_HPA (1013.25)

// Parametrer l'ecran OLED 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Declarations d'objets
TwoWire I2CBME = TwoWire(0);
Adafruit_BME280 bme; // Declaration objet bme 
RTC_DS3231 rtc;      // Declration object rtc


void displayValues();  // Declaration fonction pour affichage les valeur sur l'ecran OLED
void displayTime();    // Declaration Fonction pour affichage du temps sur l'ecran OLED

// Declaration Variables 
unsigned long  TempsActuel, TempsAvant ;
const unsigned long DELAY_TIME = 5000, DELAY_CHANGE = 10000;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; 


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


    if (rtc.lostPower()) {
      Serial.println("RTC lost power, let's set the time!");
      // When time needs to be set on a new device, or after a power loss, the
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call: rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }


    Serial.println("-- Default Test --");
    Serial.println();

    display.clearDisplay();
    display.setTextColor(WHITE);

}

void loop() { 

  TempsActuel = millis();
  
  // Affichage de la temperature et humidity 
  if (TempsActuel - TempsAvant  >= DELAY_CHANGE) {
       displayValues();   // Appeler la fonction pour afficher la temperature sur l'ecran OLED 
       delay(DELAY_TIME);
       TempsAvant = TempsActuel;
    } 
  
  // Appel\la fonction popur afficher le temps sur l'ecran OLED 
  displayTime();
}

// Implementation de la fonction d'affichage de valeur 
void displayValues() {
  
  display.clearDisplay();  // effacer l'ecran 

  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(String(bme.readTemperature()));  // Recupurer la donnee de temperature et l'afficher 
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(String(bme.readHumidity())); // Recupurer la donnee de l'humidity et l'afficher 
  display.print(" %"); 
  display.display();

}

// Implemetation de la fonction d'affichage du temps 
void displayTime() {

    DateTime now = rtc.now(); // Declaration object Now qui recois les donnees actuelle de la part de l'object rtc 

    display.clearDisplay(); 

    // display time 
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Time : ");
    display.setTextSize(2);
    display.setCursor(0,10);
    display.print(now.hour(), DEC);   // recuperer et afficher l'heure actuelle 
    display.print(':');
    display.print(now.minute(), DEC);   // recuperer et afficher les minutes 
    display.print(':');
    display.print(now.second(), DEC);  // recuperer et afficher les secondes

    // display Date
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Date : ");
    display.setTextSize(1);
    display.setCursor(0, 45);
    display.print(now.year(), DEC);  // recuperer et afficher l'annee actuelle
    display.print('/');
    display.print(now.month(), DEC); // recuperer et afficher le mois actuel 
    display.print('/');
    display.print(now.day(), DEC);  // recuperer et afficher le jour actuel 
    display.print(" (");
    display.print(daysOfTheWeek[now.dayOfTheWeek()]);  // afficher le nom du jour 
    display.print(") ");

    display.display();  // appeler la fonction pour afficher 
    
}